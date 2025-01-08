import pickle

import cv2
import numpy as np
import sys

from finger_landmarks import finger_landmarks_pb2
from finger_landmarks.finger_tracking_results import Results, Hand, Point


def to_point(point: Point):
    return finger_landmarks_pb2.Point(x=point.x, y=point.y)


def project(point: Point, persp_matrix) -> Point:
    to_transform = np.array([[[point.x, point.y]]], dtype=np.float32)
    proj = cv2.perspectiveTransform(to_transform, persp_matrix)
    result = Point(x=int(round(proj[0][0][0])), y=int(round(proj[0][0][1])))
    return result


def to_hand(hand: Hand, persp_matrix):
    proj = lambda point: project(point, persp_matrix)

    result = finger_landmarks_pb2.Hand(
        thumb=to_point(proj(hand.thumb)),
        index=to_point(proj(hand.index)),
        middle=to_point(proj(hand.middle)),
        ring=to_point(proj(hand.ring)),
        pinky=to_point(proj(hand.pinky))
    )
    return result;


if __name__ == '__main__':
    results: Results
    with open(sys.argv[1], "rb") as infile:
        results = pickle.load(infile)

    landmarks = finger_landmarks_pb2.FingerLandmarks()

    corners = np.array(
        [results.top_left, results.top_right, results.bottom_right, results.bottom_left],
        dtype="float32")
    kb_corners = np.array([(0, 0), (283, 0), (283, 94), (0, 94)], dtype="float32")
    persp_matrix = cv2.getPerspectiveTransform(corners, kb_corners)

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
            frame.leftHand.CopyFrom(to_hand(hands.left, persp_matrix))
        if hands.right is not None:
            frame.rightHand.CopyFrom(to_hand(hands.right, persp_matrix))

        landmarks.frames.append(frame)

    with open(sys.argv[2], "wb") as outfile:
        outfile.write(landmarks.SerializeToString())
        outfile.close()
