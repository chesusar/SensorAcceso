import numpy as np
import cv2
import os

BASE_DIR = "app/"
USER = "0"


def capturar_cara():

    faceCascade = cv2.CascadeClassifier(
        BASE_DIR + "cascades/haarcascade_frontalface_default.xml"
    )
    cap = cv2.VideoCapture(0)

    captures_count = 0

    while True:
        ret, img = cap.read()
        img = cv2.flip(img, -1)  # flip horizontal
        img = cv2.flip(img, 0)  # flip vertical

        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        faces = faceCascade.detectMultiScale(
            gray, scaleFactor=1.2, minNeighbors=5, minSize=(20, 20)
        )

        roi_gray = None
        cara_detectada = False

        for x, y, w, h in faces:
            cv2.rectangle(img, (x, y), (x + w, y + h), (68, 176, 238), 2)
            roi_gray = gray[y : y + h, x : x + w]
            roi_color = img[y : y + h, x : x + w]
            cara_detectada = True

        cv2.imshow("video", img)
        k = cv2.waitKey(30) & 0xFF
        if k == 27:  # press 'ESC' to quit
            break
        elif k == ord("c"):
            if cara_detectada:
                if not os.path.exists(BASE_DIR + "dataset"):
                    os.makedirs(BASE_DIR + "dataset")
                cv2.imwrite(
                    BASE_DIR + "dataset/User_" + USER + "_" + str(captures_count) + ".jpg",
                    roi_gray,
                )
                captures_count += 1
    cap.release()
    cv2.destroyAllWindows()


capturar_cara()
