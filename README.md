<h1 align="center">
  <a href="https://youtu.be/GM8qZVZJs2o"><img src="https://github.com/TrashRobotics/ScrewPropelledVehicle/blob/main/img/screw_propelled_vehicle.jpg" alt="Шнекороторный снегоболотоход" width="800"></a>
  <br>
    Шнековая платформа
  <br>
</h1>

<p align="center">
  <a href="https://github.com/TrashRobotics/ScrewPropelledVehicle/blob/main/README.md">Русский</a> •
  <a href="https://github.com/TrashRobotics/ScrewPropelledVehicle/blob/main/README-en.md">English(Английский)</a> 
</p>

# Описание проекта
Код для тестирования шнековой платформы

# Основные детали
* Arduino pro mini;
* 2 x 18650 Аккумуляторы и батарейный отсек для них
* тумблер KCD-11;
* L298N Драйвер двигателей;
* желтые ардуино TT-мотор-редукторы;
* HW-517 или другой транзисторный ключ;
* HC-06 блютуз модуль (опционально);
* KY-022 ИК-приемник и пульт для него;
* кусок фанеры 305x170x6 мм;
* [шнек, держатели, кабина и т.д.](https://www.thingiverse.com/thing:5204085)

### Крепеж
* Подшипник 623 2RS (180023) 3dx10Dx4H (мм) x2;
* Саморез DIN7982 3.5x9.5	x2;
* Саморез DIN7982 2.9x16	x12;
* Саморез DIN7981 2.2x9.5	x12;
* Саморез DIN7981 2.9x9.5	x12;
* Винт DIN7985 M3x25		x6;
* Гайка DIN934 M3x0.5		x8;

# Схема подключения
![Схема подключения](https://github.com/TrashRobotics/ScrewPropelledVehicle/blob/main/img/schematic.png)

# Софт
Прошивка для Arduino pro mini: **screw_propelled_vehicle/screw_propelled_vehicle.ino**
Если планируется осуществлять управление с ИК-пульта, то в самом скетче необходимо поменять коды кнопок под ваш ИК-пульт.

**bluetoothtester.py** - опциональный скрипт на python для управления платформой с ПК.
Для запуска скрипта необходимо установить пакет punput.
Управление платформой с ПК осуществляется стрелочками.

Другие подробности смотрите в видео.

