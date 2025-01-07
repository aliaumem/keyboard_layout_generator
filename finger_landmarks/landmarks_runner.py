import cv2 as cv
import mediapipe as mp

from finger_landmarks.draw_landmarks_annotations import draw_landmarks_on_image
from finger_landmarks.keyboard_shape_recorder import KeyboardShapeRecorder
from finger_landmarks.rotation_enum import Rotation

BaseOptions = mp.tasks.BaseOptions
HandLandmarker = mp.tasks.vision.HandLandmarker
HandLandmarkerOptions = mp.tasks.vision.HandLandmarkerOptions
VisionRunningMode = mp.tasks.vision.RunningMode


class FingerLandmarksRunner:
    model_path: str
    cap: cv.VideoCapture

    timestamp: int = 0
    fps: float
    rotation: Rotation
    keyboard_rec = KeyboardShapeRecorder()
    window_name = "hand_detection"

    landmarks = list()

    def __init__(self, model_path: str, video_path: str, rotation: Rotation):
        self.cap = cv.VideoCapture(video_path)

        if not self.cap.isOpened():
            print("Could not open file")
            exit()
        self.fps = self.cap.get(cv.CAP_PROP_FPS)

        self.model_path = model_path
        self.rotation = rotation

    def run(self):
        cv.namedWindow(self.window_name)

        # Create a hand landmarker instance with the video mode:
        options = HandLandmarkerOptions(
            base_options=BaseOptions(model_asset_path=self.model_path),
            running_mode=VisionRunningMode.VIDEO,
            num_hands=2)

        with (HandLandmarker.create_from_options(options) as landmarker):
            while True:
                ret, frame = self.cap.read()
                if not ret:
                    break

                if self.rotation != Rotation.ROTATE_IDENTITY:
                    frame = cv.rotate(frame, self.rotation.value)

                if self.is_first_frame():
                    self.keyboard_rec.record_corners(self.window_name, frame)

                hand_landmarker_result = self.detect_landmarks(frame, landmarker)

                self.landmarks.append(hand_landmarker_result)

                self.draw_annotated(frame, hand_landmarker_result)

        cv.destroyAllWindows()

    def is_first_frame(self):
        return self.timestamp == 0

    def detect_landmarks(self, frame, landmarker):
        mp_image = mp.Image(image_format=mp.ImageFormat.SRGB,
                            data=cv.cvtColor(frame, cv.COLOR_RGB2BGR))

        hand_landmarker_result = landmarker.detect_for_video(mp_image, self.timestamp)
        self.timestamp += int(1000 / self.fps)

        return hand_landmarker_result

    def draw_annotated(self, frame, hand_landmarker_result):
        annotated = draw_landmarks_on_image(frame, hand_landmarker_result)
        cv.imshow(self.window_name, annotated)
        cv.waitKey(1)

    @property
    def width(self) -> int:
        return int(self.cap.get(cv.CAP_PROP_FRAME_WIDTH))

    @property
    def height(self) -> int:
        return int(self.cap.get(cv.CAP_PROP_FRAME_HEIGHT))
