import cv2
import numpy as np

def nothing(x):
    pass

cap = cv2.VideoCapture(0)


cv2.namedWindow("Suwaki")
cv2.resizeWindow("Suwaki", 300, 300)


cv2.createTrackbar("L - H", "Suwaki", 0, 179, nothing)
cv2.createTrackbar("L - S", "Suwaki", 50, 255, nothing)
cv2.createTrackbar("L - V", "Suwaki", 50, 255, nothing)
cv2.createTrackbar("U - H", "Suwaki", 179, 179, nothing)
cv2.createTrackbar("U - S", "Suwaki", 255, 255, nothing)
cv2.createTrackbar("U - V", "Suwaki", 255, 255, nothing)

print("Instrukcja:")
print("1. Przesuwaj suwaki tak, aby skrzydła Ho-Oh były BIAŁE, a tło CZARNE.")
print("2. Jak ustawisz idealnie, spisz liczby z suwaków.")
print("3. Naciśnij 'q' żeby wyjść.")

while True:
    ret, frame = cap.read()
    if not ret: break


    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    l_h = cv2.getTrackbarPos("L - H", "Suwaki")
    l_s = cv2.getTrackbarPos("L - S", "Suwaki")
    l_v = cv2.getTrackbarPos("L - V", "Suwaki")
    u_h = cv2.getTrackbarPos("U - H", "Suwaki")
    u_s = cv2.getTrackbarPos("U - S", "Suwaki")
    u_v = cv2.getTrackbarPos("U - V", "Suwaki")

    lower_color = np.array([l_h, l_s, l_v])
    upper_color = np.array([u_h, u_s, u_v])
    mask = cv2.inRange(hsv, lower_color, upper_color)


    cv2.imshow("Kamera", frame)
    cv2.imshow("Maska (To widzi bot)", mask)

    key = cv2.waitKey(1)
    if key == ord('q'):
        print("\n--- TWOJE USTAWIENIA DO SKOPIOWANIA ---")
        print(f"low_red = np.array([{l_h}, {l_s}, {l_v}])")
        print(f"high_red = np.array([{u_h}, {u_s}, {u_v}])")
        print("---------------------------------------")
        break

cap.release()
cv2.destroyAllWindows()