import cv2
import numpy as np
import serial
import time

PORT_ESP = 'COM7'   
BAUD_RATE = 115200
KAMERA_INDEX = 0


PROG_PIKSELI = 1800


LINIA_CIECIA = 0.65 

print(f"Łączenie z ESP32 na porcie {PORT_ESP}...")
try:
    ser = serial.Serial(PORT_ESP, BAUD_RATE, timeout=1)
    time.sleep(2) 
    print("POŁĄCZONO! Bot gotowy.")
except:
    print(f"BŁĄD: Nie mogę otworzyć portu {PORT_ESP}.")
    exit()

cap = cv2.VideoCapture(KAMERA_INDEX)
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 1)
cap.set(cv2.CAP_PROP_EXPOSURE, -3) 

low_red = np.array([1, 103, 68])
high_red = np.array([14, 212, 255])

print("Czekam na sygnał 'SKANUJ'...")

while True:
    if ser.in_waiting > 0:
        try:
            line = ser.readline().decode('utf-8').strip()
            
            if line == "SKANUJ":
                print("\n--- SKANOWANIE ---")
                for _ in range(5): ret, frame = cap.read()
                if not ret: break

                height, width = frame.shape[:2]
                

                granica_y = int(height * LINIA_CIECIA)
                

                frame_gora = frame[0:granica_y, 0:width]


                hsv = cv2.cvtColor(frame_gora, cv2.COLOR_BGR2HSV)
                mask = cv2.inRange(hsv, low_red, high_red)
                
                red_count = cv2.countNonZero(mask)
                print(f"Wykryto punktów (nad linią): {red_count}")


                if red_count > PROG_PIKSELI:
                    print(">>> DECYZJA: ZWYKŁY HO-OH. RESETUJĘ!")
                    ser.write(b'r') 
                else:
                    print(f">>> DECYZJA: MAŁO KOLORU ({red_count})! SHINY?! STOP!")
                    ser.write(b's')
                    cv2.imwrite("SHINY_ALBO_BLAD.jpg", frame)
                    while True: time.sleep(1) 

        except Exception as e:
            print(f"Błąd: {e}")


    ret, frame = cap.read()
    if ret:
        height, width = frame.shape[:2]
        granica_y = int(height * LINIA_CIECIA)


        cv2.line(frame, (0, granica_y), (width, granica_y), (255, 0, 0), 3)


        frame_gora = frame[0:granica_y, 0:width] 
        hsv_live = cv2.cvtColor(frame_gora, cv2.COLOR_BGR2HSV)
        mask_live = cv2.inRange(hsv_live, low_red, high_red)
        count_live = cv2.countNonZero(mask_live)

        color_status = (0, 0, 255) if count_live > PROG_PIKSELI else (0, 255, 0)
        
        cv2.putText(frame, f"Punkty : {count_live}", (10, 30), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, color_status, 2)
        cv2.putText(frame, "", (10, height - 20), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)

        cv2.imshow("Kamera", frame)
        cv2.imshow("Maska ", mask_live)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
ser.close()