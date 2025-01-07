import pickle
import sys

from finger_landmarks.finger_tracking_results import Results, Hand, Point
from finger_landmarks import finger_landmarks_pb2


def to_point(point: Point):
    return finger_landmarks_pb2.Point(x=point.x, y=point.y)


def to_hand(hand: Hand):
    return finger_landmarks_pb2.Hand(
        thumb=to_point(hand.thumb),
        index=to_point(hand.index),
        middle=to_point(hand.middle),
        ring=to_point(hand.ring),
        pinky=to_point(hand.pinky)
    )


if __name__ == '__main__':
    results: Results
    with open(sys.argv[1], "rb") as infile:
        results = pickle.load(infile)

    landmarks = finger_landmarks_pb2.FingerLandmarks()

    landmarks.keyboardCorners.CopyFrom(finger_landmarks_pb2.KeyboardCorners(
        topLeft=to_point(Point._make(results.top_left)),
        topRight=to_point(Point._make(results.top_right)),
        bottomRight=to_point(Point._make(results.bottom_right)),
        bottomLeft=to_point(Point._make(results.bottom_left))
    ))

    print(len(results.hands))
    for hands in results.hands:
        frame = finger_landmarks_pb2.Frame()
        frame.timestamp = hands.timestamp
        if hands.left is not None:
            frame.leftHand.CopyFrom(to_hand(hands.left))
        if hands.right is not None:
            frame.rightHand.CopyFrom(to_hand(hands.right))

        landmarks.frames.append(frame)

    with open(sys.argv[2], "wb") as outfile:
        outfile.write(landmarks.SerializeToString())
        outfile.close()
