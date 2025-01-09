import pickle

import sys

from finger_landmarks.finger_tracking_results import Point, Hand, BothHands, Results
from finger_landmarks.landmarks_runner import FingerLandmarksRunner
from finger_landmarks.rotation_enum import Rotation, parse_rotation


def populate_results(runner: FingerLandmarksRunner) -> Results:
    result = Results()
    result.fps = runner.fps

    to_point = lambda landmark: Point(x=int(landmark.x * runner.width),
                                      y=int(landmark.y * runner.height))

    for i, detection_result in enumerate(runner.landmarks):
        left_hand: Hand | None = None
        right_hand: Hand | None = None

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

        result.add_hands(BothHands(left=left_hand, right=right_hand))
    return result


def run(filename: str, chunk_id: int, chunk_count: int, rotation: Rotation):
    runner = FingerLandmarksRunner(
        # model_path='finger_landmarks/hand_landmarker.task',
        model_path='C:/prog/finger_tracking/finger_landmarks/hand_landmarker.task',
        video_path=filename,
        chunk_id=chunk_id,
        chunk_count=chunk_count,
        rotation=rotation)
    runner.run()

    result: Results = populate_results(runner)

    with open(filename.rstrip(".mp4") + f"_{chunk_id}_landmarks.pkl", "wb") as outfile:
        pickle.dump(result, outfile)
        outfile.close()


if __name__ == '__main__':
    if len(sys.argv) < 4:
        print(f"Usage: {sys.argv[0]} filename.mp4 chunk_id chunk_count [rotation]")
        exit()

    chunk_id = int(sys.argv[2])
    chunk_count = int(sys.argv[3])
    rotation = parse_rotation(sys.argv[4]) if len(sys.argv) > 4 else Rotation.ROTATE_IDENTITY

    run(sys.argv[1], chunk_id, chunk_count, rotation)
