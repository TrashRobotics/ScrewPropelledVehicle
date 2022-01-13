import socket
import json
from pynput import keyboard     # нужно установить для управления с клавиатуры
import time

msg = {
    "speedA": 0,    # в пакете посылается скорость на левый и правый борт тележки
    "speedB": 0     #
}

speedScale = 1.00   # определяет скорость в процентах (0.90 = 90%) от максимальной абсолютной
maxAbsSpeed = 100   # максимальное абсолютное отправляемое значение скорости
sendFreq = 15       # слать 15 пакетов в секунду


def onPress(key):
    """ вызывается при нажатии какой либо клавиши на клавиатуре """
    global msg
    global speedScale, maxAbsSpeed
    if key == keyboard.Key.up:      # управление стрелочками
        msg["speedA"] = maxAbsSpeed * speedScale
        msg["speedB"] = maxAbsSpeed * speedScale
    elif key == keyboard.Key.down:
        msg["speedA"] = -maxAbsSpeed * speedScale
        msg["speedB"] = -maxAbsSpeed * speedScale
    elif key == keyboard.Key.left:
        msg["speedA"] = maxAbsSpeed * speedScale
        msg["speedB"] = maxAbsSpeed * speedScale * 0.75
    elif key == keyboard.Key.right:
        msg["speedA"] = maxAbsSpeed * speedScale * 0.75
        msg["speedB"] = maxAbsSpeed * speedScale


def onRelease(key):
    """ вызывается при отпускании какой либо клавиши на клавиатуре """
    global msg
    global speedScale, maxAbsSpeed
    if key == keyboard.Key.up:      # управление стрелочками
        msg["speedA"] = 0
        msg["speedB"] = 0
    elif key == keyboard.Key.down:
        msg["speedA"] = 0
        msg["speedB"] = 0
    elif key == keyboard.Key.left:
        msg["speedA"] = 0
        msg["speedB"] = 0
    elif key == keyboard.Key.right:
        msg["speedA"] = 0
        msg["speedB"] = 0


if __name__ == '__main__':
    listener = keyboard.Listener(
        on_press=onPress,
        on_release=onRelease)
    listener.start()    # запускаем обработчик нажатия клавиш в неблокирующем режиме

    sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)  # on python 3.9
    sock.connect(("00:21:13:04:d9:63", 1))  # подключаемся к блютуз модулю HC-06 по mac-адресу

    try:
        while True:
            sock.send(json.dumps(msg, ensure_ascii=False).encode("utf8"))   # отправляем сообщение в виде json файла
            #print(sock.recv(1024))
            time.sleep(1 / sendFreq)
    except KeyboardInterrupt:
        sock.close()
        listener.stop()
