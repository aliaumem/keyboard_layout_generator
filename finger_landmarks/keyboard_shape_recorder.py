import string
from typing import Sequence
import numpy as np
import cv2 as cv


class KeyboardShapeRecorder:
    points = np.zeros(4, dtype=type(Sequence[int]))
    index = 0

    def mouse_click(self, event, x, y):
        if event != cv.EVENT_LBUTTONDOWN:
            return
        self.points[self.index] = x, y
        self.index += 1

    def print_markers(self, frame: np.ndarray):
        annotated = frame.copy()
        for i in range(0, self.index):
            cv.drawMarker(annotated, self.points[i], (88, 205, 54))
        return annotated

    def record_corners(self, window_name: string, frame: np.ndarray):

        cv.setMouseCallback(window_name,
                            lambda event, x, y, flags, params: self.mouse_click(event, x, y))

        for i in range(0, 4):
            annotated = self.print_markers(frame)
            cv.imshow(window_name, annotated)
            while self.index < i + 1:
                cv.waitKey(100)

        cv.setMouseCallback(window_name, lambda event, x, y, flags, params: None)
