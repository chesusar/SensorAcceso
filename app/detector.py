import cv2
import numpy as np
import os
from Adafruit_IO import MQTTClient
from credenciales import ADAFRUIT_IO_KEY
from credenciales import ADAFRUIT_IO_USERNAME
import time


ORDEN_USUARIO = 0
TIEMPO_MINIMO_MQTT = 15


BASE_DIR = "app/"


def detect():

    recognizer = cv2.face.LBPHFaceRecognizer_create()
    recognizer.read(BASE_DIR + "model/model.yml")
    cascadePath = BASE_DIR + "cascades/haarcascade_frontalface_default.xml"
    faceCascade = cv2.CascadeClassifier(cascadePath)

    font = cv2.FONT_HERSHEY_SIMPLEX

    id = 1
    names = ["Usuario"]

    cam = cv2.VideoCapture(0)

    # Define min window size to be recognized as a face
    minW = 0.1 * cam.get(3)
    minH = 0.1 * cam.get(4)

    # Create an MQTT client instance.
    client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

    # Connect to the Adafruit IO server.
    client.connect()
    client.loop_background()

    tiempo_ultimo_mqtt = 0

    while True:
        ret, img = cam.read()

        img = cv2.flip(img, -1)  # flip horizontal
        img = cv2.flip(img, 0)  # flip vertical

        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        faces = faceCascade.detectMultiScale(
            gray,
            scaleFactor=1.2,
            minNeighbors=5,
            minSize=(int(minW), int(minH)),
        )

        for x, y, w, h in faces:

            id, confidence = recognizer.predict(gray[y : y + h, x : x + w])

            # Check if confidence is less them 100 ==> "0" is perfect match
            if confidence < 50:
                cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
                id = names[id]
                confidence = "  {0}%".format(round(100 - confidence))
                if time.time() - tiempo_ultimo_mqtt > TIEMPO_MINIMO_MQTT:
                    enviar_mqtt(client=client, orden=ORDEN_USUARIO)
                    tiempo_ultimo_mqtt = time.time()

            else:
                cv2.rectangle(img, (x, y), (x + w, y + h), (0, 0, 255), 2)
                id = "Intruso"
                confidence = "  {0}%".format(round(100 - confidence))
                # if time.time() - tiempo_ultimo_mqtt > TIEMPO_MINIMO_MQTT:
                #     enviar_mqtt(client=client, orden=ORDEN_INTRUSO)
                #     tiempo_ultimo_mqtt = time.time()

            cv2.putText(img, str(id), (x + 5, y - 5), font, 1, (255, 255, 255), 2)
            # cv2.putText(img, str(confidence), (x + 5, y + h - 5), font, 1, (255, 255, 0), 1)

        cv2.imshow("Detecctor", img)

        k = cv2.waitKey(10) & 0xFF  # Press 'ESC' for exiting video
        if k == 27:
            break

    cam.release()
    cv2.destroyAllWindows()


def enviar_mqtt(client, orden):
    client.publish("orden", orden)


detect()
