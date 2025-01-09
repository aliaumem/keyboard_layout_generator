from functools import total_ordering

import cv2 as cv
import mediapipe as mp
import time
import math

from finger_landmarks.draw_landmarks_annotations import draw_landmarks_on_image
from finger_landmarks.rotation_enum import Rotation

BaseOptions = mp.tasks.BaseOptions
HandLandmarker = mp.tasks.vision.HandLandmarker
HandLandmarkerOptions = mp.tasks.vision.HandLandmarkerOptions
HandLandmarkerResult = mp.tasks.vision.HandLandmarkerResult
VisionRunningMode = mp.tasks.vision.RunningMode


class FingerLandmarksRunner:
    model_path: str
    cap: cv.VideoCapture

    chunk_id: int
    chunk_count: int

    timestamp: int = 0
    fps: float
    total_count: int
    rotation: Rotation

    landmarks = list()
    timestamps = list()

    def __init__(self, model_path: str, video_path: str, chunk_id: int, chunk_count: int,
                 rotation: Rotation):
        self.chunk_id = chunk_id
        self.chunk_count = chunk_count
        self.cap = cv.VideoCapture(video_path)

        if not self.cap.isOpened():
            print("Could not open file")
            exit()
        self.fps = self.cap.get(cv.CAP_PROP_FPS)

        self.model_path = model_path
        self.rotation = rotation

    def on_result(self, hand_landmarker_result, output_image, timestamp_ms):
        self.landmarks.append(hand_landmarker_result)
        self.timestamps.append(timestamp_ms)
        print(f"{len(self.landmarks)} / {self.total_count}", flush=True)

    def run(self):
        callback = lambda result, out_image, timestamp_ms: self.on_result(result, out_image,
                                                                          timestamp_ms)

        video_frame_count = int(self.cap.get(cv.CAP_PROP_FRAME_COUNT))
        self.total_count = int(math.ceil(video_frame_count / self.chunk_count))
        if self.chunk_id == self.chunk_count - 1:
            self.total_count = self.total_count - self.total_count % self.chunk_count

        # Create a hand landmarker instance with the video mode:
        options = HandLandmarkerOptions(
            base_options=BaseOptions(model_asset_path=self.model_path),
            running_mode=VisionRunningMode.LIVE_STREAM,
            result_callback=callback,
            num_hands=2)

        with (HandLandmarker.create_from_options(options) as landmarker):
            for i in range(self.chunk_id * self.total_count):
                self.cap.grab()  # skips frames until we get to the start of our chunk

            for i in range(self.total_count):
                ret, frame = self.cap.read()
                if not ret:
                    break

                if self.rotation != Rotation.ROTATE_IDENTITY:
                    frame = cv.rotate(frame, self.rotation.value)

                mp_image = mp.Image(image_format=mp.ImageFormat.SRGB,
                                    data=cv.cvtColor(frame, cv.COLOR_RGB2BGR))
                landmarker.detect_async(mp_image, self.timestamp)
                self.timestamp += int(1000 / self.fps)

        while len(self.landmarks) < self.total_count:
            time.sleep(1)

    @property
    def width(self) -> int:
        if self.rotation in (Rotation.ROTATE_IDENTITY, Rotation.ROTATE_180):
            return int(self.cap.get(cv.CAP_PROP_FRAME_WIDTH))
        else:
            return int(self.cap.get(cv.CAP_PROP_FRAME_HEIGHT))

    @property
    def height(self) -> int:
        if self.rotation in (Rotation.ROTATE_IDENTITY, Rotation.ROTATE_180):
            return int(self.cap.get(cv.CAP_PROP_FRAME_HEIGHT))
        else:
            return int(self.cap.get(cv.CAP_PROP_FRAME_WIDTH))
