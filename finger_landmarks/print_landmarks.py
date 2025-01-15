import cv2
import sys

from finger_landmarks import finger_landmarks_pb2
from finger_landmarks.rotation_enum import Rotation, parse_rotation

if __name__ == "__main__":
    video_path = sys.argv[1]
    base_path = video_path.rstrip(".mp4")
    cap = cv2.VideoCapture(video_path)

    rotation: Rotation = parse_rotation(sys.argv[2])

    landmarks = finger_landmarks_pb2.FingerLandmarks()
    with open(base_path + "_landmarks.binarypb", "rb") as f:
        landmarks.ParseFromString(f.read())

    wait_time = 33
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        if rotation is not Rotation.ROTATE_IDENTITY:
            frame = cv2.rotate(frame, rotation.value)

        cv2.putText(frame, f"pos: {cap.get(cv2.CAP_PROP_POS_MSEC)}ms", (20, 20),
                    cv2.FONT_HERSHEY_PLAIN, 1, color=(200, 200, 200))

        cv2.imshow("landmarks", frame)
        key = cv2.waitKeyEx(wait_time)
        if key != -1:
            if key == 2424832:  # previous
                pos = cap.get(cv2.CAP_PROP_POS_FRAMES)
                cap.set(cv2.CAP_PROP_POS_FRAMES, pos - 2)
            elif key == 2555904:  # next
                pass
            elif key == 32:  # space
                wait_time = 0 if wait_time != 0 else 33
            print(key)
