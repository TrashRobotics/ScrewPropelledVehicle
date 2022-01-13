#include <IRremote.h>
#include "ArduinoJson.h"

// коды управления платформой с ИК-пульта
#define MOVE_FORWARD_IR_CODE    0x20DF02FD  // движение вперед 
#define MOVE_BACKWARD_IR_CODE   0x20DF827D  // движение назад
#define MOVE_FL_IR_CODE         0x20DFE01F  // движение вперед с поворотом влево
#define MOVE_FR_IR_CODE         0x20DF609F  // движение вперед с поворотом вправо
#define MOVE_LEFT_IR_CODE       0x20DFF10E  // движение влево (без движения вперед/назад)
#define MOVE_RIGHT_IR_CODE      0x20DF718E  // движение вправо (без движения вперед/назад)
#define SET_HEADLIGHTS_IR_CODE  0x20DF10EF  // включить/выключить фары

#define IR_SPEED  255 // скорость платформы при управлении с ИК-пульта [0-255]

#define IN1_PIN   8   // пины драйвера двигателей l298n
#define IN2_PIN   7   //
#define IN3_PIN   4   //
#define IN4_PIN   3   //  
#define ENA_PIN   5   //
#define ENB_PIN   6   //  

#define IR_PIN    11  // пин ИК-приемника

#define HEADLIGHTS_PIN  12  // пин транзисторного ключа для включения/выключения фар

#define VOLTAGE_PIN     A6      // пин для считывания напряжения с li-ion 
#define VOLTAGE_REF     4.8f    // питающее напряжение 
#define VOLTAGE_GAIN    3.20f   // коэффициент передачи делителя напряжения 8V <-> 2.5V
#define VOLTAGE_OFFSET  0       // 60  // ошибка измерения аналогового выхода 
#define MIN_SUPPLY_VOLTAGE 6.65f // минимальное напряжение разряда АКБ (ниже 6.5В не ставить. т.к. регулятор напряжения 
// на l298n при питании ниже 6.5В выдает на выходе меньше 5В напряжения для питания arduino, из-за чего опорное напряжение падает и 
// считывание с аналогового выхода идет с ошибкой. Как решение можно поставить доп. стабилизатор или регулятор, чтоб питать arduino отдельно)
#define LOW_VOLTAGE_WARNING_MSG_PERIOD 15000 // в миллисекундах (через сколько моргают фары)

StaticJsonDocument<200> jsondoc;  // json буфер для пакетов от ПК
IRrecv ir(IR_PIN);  // экзепляр для работы с ИК-приемником
decode_results irResults;

//состояния конечного автомата платформы
enum FsmStates  
{
  LOW_VOLTAGE,  // прерывает работу, уходит в полуспящий режим
  WORK, // режимо обычной работы 
  ERROR // если появилась какая-то ошибка
};

const float speedFactorA = 1.00;  // дополнительные параметры компенсирующие разницу характеристик моторов
const float speedFactorB = 0.95;  //

// глобальные переменные
FsmStates state = FsmStates::WORK;
uint32_t lowVoltageTimer = 0; // таймер сообщения о низком заряде
int16_t speedA = 0;
int16_t speedB = 0;


void setup() {
  // настройка переферии
  Serial.begin(115200);
  pinMode(IN1_PIN, OUTPUT);    
  pinMode(IN2_PIN, OUTPUT);    
  pinMode(IN3_PIN, OUTPUT);    
  pinMode(IN4_PIN, OUTPUT);    
  pinMode(ENA_PIN, OUTPUT);    
  pinMode(ENB_PIN, OUTPUT);      
  pinMode(HEADLIGHTS_PIN, OUTPUT);  
  pinMode(VOLTAGE_PIN, INPUT);
  ir.enableIRIn();
  
  moveA(0);
  moveB(0);
  setHeadlights(true);
  while (Serial.available() > 0) Serial.read(); // чистим serial буфер
}

void loop() {
  //Serial.println(getVoltage());
  switch(state){  // конечный автомат
      case FsmStates::WORK:{    // рабочее состояние
        if(getVoltage() < MIN_SUPPLY_VOLTAGE){  // если напряжение ниже положенного
          state = FsmStates::LOW_VOLTAGE;   // переходим в режим низкого напряжения
          moveA(0);
          moveB(0);
          setHeadlights(false);
          delay(500);
          break; // останавливаем работу
        }
        checkIR();     // проверка на сигналы управления с ИК-пульта
        checkSerial(); // проверка на сигналы управления с HC-06
        moveA((float)speedA*speedFactorA);  // двигаемся куда нужно
        moveB((float)speedB*speedFactorB);  //
        delay(10);
        break;
      }
      
      case FsmStates::LOW_VOLTAGE:{ // переодически моргает фарами (3 раза), говоря, что АКБ сел
        if ((millis() > lowVoltageTimer + LOW_VOLTAGE_WARNING_MSG_PERIOD) || (lowVoltageTimer == 0)){
          moveA(0);
          moveB(0);
          setHeadlights(false);
          delay(500);

          for (uint8_t i = 0; i < 6; i++){   // моргаем фарами 3 раза
            setHeadlights(i % 2 == 0);
            delay(500);
          }
          
          setHeadlights(false);
          
          lowVoltageTimer = millis();
        }
        break;
      }  

      case FsmStates::ERROR:{
        break;
      }    
      
      default: {
        break;
      }
  }
}


void moveA(int16_t speed){  // движение одного из моторов (А)
  speed = -speed; // инвертируем напраление вращения
  analogWrite(ENA_PIN, abs(speed));
  if(speed >= 0){
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);  
  } 
  else {
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
  }
}

void moveB(int16_t speed){  // движение одного из моторов (B)
  speed = -speed; // инвертируем напраление вращения
  analogWrite(ENB_PIN, abs(speed));
  if(speed >= 0){
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, HIGH);  
  } 
  else {
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);
  }
}


float getVoltage(){ // получение напряжения по показаниям аналогового выхода
  return (float)(constrain((int16_t)analogRead(VOLTAGE_PIN) - VOLTAGE_OFFSET, 0, 1024)) * VOLTAGE_REF * VOLTAGE_GAIN / 1024.f;
} 


void checkSerial(){  // проверяем serial на наличие комманд управления (json)
  if(Serial.available()){
    DeserializationError err = deserializeJson(jsondoc, Serial);  // получаем пакеты по uart через HC-06
    if (err == DeserializationError::Ok)    // если пакет принят
    {
      speedA = (float)jsondoc["speedA"] * 2.55;  // приходит диапазон [-100; 100]
      speedB = (float)jsondoc["speedB"] * 2.55;  // расширяем до [-255; 255]
    }
    else
    {
      while (Serial.available() > 0) Serial.read(); // чистим буфер
    }
  }
}


void checkIR(){   // проверяем ИК-приемник на наличие сигналов управления
  static bool headlightsState = true;   // переменная, хранящее прошлое значение состояния фар, принятое через ir
  static uint32_t irResetTimer = millis();   // таймер сброса движения платформы, если нет ужержания кнопок на пульте
  static bool isRecv = false; // флаг - приходили ли новые данные
  
  if (ir.decode(&irResults)) { // если данные пришли
    //Serial.println(irResults.value, HEX); //вывод результатов в монитор порта
    isRecv = true;
    switch (irResults.value) {
      case MOVE_FORWARD_IR_CODE: {
        speedA = IR_SPEED;
        speedB = IR_SPEED;
        irResetTimer = millis();
        break;
      }
      
      case MOVE_BACKWARD_IR_CODE: {
        speedA = -IR_SPEED;
        speedB = -IR_SPEED;
        irResetTimer = millis();    
        break;
      }

      case MOVE_FL_IR_CODE: {
        speedA = IR_SPEED;
        speedB = IR_SPEED * 0.75;
        irResetTimer = millis();
        break;
      }

      case MOVE_FR_IR_CODE: {
        speedA = IR_SPEED * 0.75;
        speedB = IR_SPEED;
        irResetTimer = millis();
        break;
      }
      
      case MOVE_LEFT_IR_CODE: {
        speedA = -IR_SPEED;
        speedB = IR_SPEED;
        irResetTimer = millis();
        break;
      }

      case MOVE_RIGHT_IR_CODE: {
        speedA = IR_SPEED;
        speedB = -IR_SPEED;
        irResetTimer = millis();
        break;
      }

      case SET_HEADLIGHTS_IR_CODE: {
        setHeadlights(headlightsState);
        headlightsState = !headlightsState;
        break;
      }

      case 0xFFFFFFFF: {  // удержиние
        irResetTimer = millis();
        break;
      }
    }    
    ir.resume(); // принимаем следующую команду
  }

  if ((millis() > irResetTimer + 300) && (isRecv)) {   // проверяет раз в полсекунды (проверка на удержание кнопок)
    speedA = 0;
    speedB = 0;
    isRecv = false;
    irResetTimer = millis();
  }
}


void setHeadlights(bool state){   // включает/выключает фары
  if (state) digitalWrite(HEADLIGHTS_PIN, HIGH);
  else  digitalWrite(HEADLIGHTS_PIN, LOW);
}
