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


def compute_perspective(file_basename: str):
    corners_proto = finger_landmarks_pb2.KeyboardCorners()

    with open(file_basename + "_corners.binarypb", "rb") as corners_f:
        corners_proto.ParseFromString(corners_f.read())

    from_proto = lambda proto_point: (proto_point.x, proto_point.y)

    corners = np.array(
        [from_proto(corners_proto.topLeft),
         from_proto(corners_proto.topRight),
         from_proto(corners_proto.bottomRight),
         from_proto(corners_proto.bottomLeft)],
        dtype="float32")

    kb_corners = np.array([(0, 0), (283, 0), (283, 94), (0, 94)], dtype="float32")

    return cv2.getPerspectiveTransform(corners, kb_corners)


if __name__ == '__main__':
    file_basename = sys.argv[1].strip("_landmarks.pkl")

    results: Results
    with open(sys.argv[1], "rb") as infile:
        results = pickle.load(infile)

    persp_matrix = compute_perspective(file_basename)

    landmarks = finger_landmarks_pb2.FingerLandmarks()
    print(len(results.hands))
    for hands in results.hands:
        frame = finger_landmarks_pb2.Frame()
        frame.timestamp = hands.timestamp
        if hands.left is not None:
            frame.leftHand.CopyFrom(to_hand(hands.left, persp_matrix))
        if hands.right is not None:
            frame.rightHand.CopyFrom(to_hand(hands.right, persp_matrix))

        landmarks.frames.append(frame)

    with open(file_basename + "_landmarks.binarypb", "wb") as outfile:
        outfile.write(landmarks.SerializeToString())
        outfile.close()
