import pickle

import sys

from finger_landmarks.finger_tracking_results import Point, Hand, BothHands, Results
from finger_landmarks.landmarks_runner import FingerLandmarksRunner
from finger_landmarks.rotation_enum import Rotation

if __name__ == '__main__':
    runner = FingerLandmarksRunner(model_path='finger_landmarks/hand_landmarker.task',
                                   video_path=sys.argv[1],
                                   rotation=Rotation.ROTATE_90_COUNTERCLOCKWISE)
    runner.run()

    resolution = (runner.width, runner.height)
    result = Results()
    result.set_corners(top_left=runner.keyboard_rec.points[0],
                       top_right=runner.keyboard_rec.points[1],
                       bottom_right=runner.keyboard_rec.points[2],
                       bottom_left=runner.keyboard_rec.points[3])
    result.fps = runner.fps

    to_point = lambda landmark: Point(x=int(landmark.x * resolution[0]),
                                      y=int(landmark.y * resolution[1]))

    for i, detection_result in enumerate(runner.landmarks):
        left_hand: Hand | None = None
        right_hand: Hand | None = None
        timestamp = int((1000 * i) / result.fps)
        for landmarks, handedness in zip(detection_result.hand_landmarks,
                                         detection_result.handedness):
            hand = Hand(thumb=to_point(landmarks[4]),
                        index=to_point(landmarks[8]),
                        middle=to_point(landmarks[12]),
                        ring=to_point(landmarks[16]),
                        pinky=to_point(landmarks[20]))

            if handedness[0].category_name == "Left":
                left_hand = hand
            else:
                right_hand = hand

        result.add_hands(BothHands(left=left_hand, right=right_hand, timestamp=timestamp))

    with open(sys.argv[2], "wb") as outfile:
        pickle.dump(result, outfile)
        outfile.close()
