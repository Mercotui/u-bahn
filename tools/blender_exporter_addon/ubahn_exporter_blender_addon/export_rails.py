#  SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
#  SPDX-License-Identifier: MIT
from itertools import combinations, product
from math import isclose

from numpy import array
from numpy.linalg import norm

from . import world_pb2

type Point = list[float]
type Vector = list[Point]


def serialize(section):
    return section.SerializeToString()


def approximately_equal(a: Point, b: Point):
    """
    Check if the first control points are approximately the same vertex, with a 30 centimeter radius as tolerance.
    :param a: point a
    :param b: point b
    :return: true if point a and b are within 30 centimeters of each-other, otherwise false
    """
    tol = 0.3  # mega toll
    return (
        isclose(a[0], b[0], abs_tol=tol)
        and isclose(a[1], b[1], abs_tol=tol)
        and isclose(a[2], b[2], abs_tol=tol)
    )


def are_antiparallel(a: Vector, b: Vector):
    """
    Check if the given two vectors are antiparallel, i.e. they point into each-other, with a tolerance of 40
    :param a: vector a
    :param b: vector b
    :return: true if the two vectors are antiparallel within a tolerance, otherwise false
    """
    # Use numpy arrays to get the dot product
    array_a = array(a[0]) - array(a[1])
    array_b = array(b[0]) - array(b[1])
    cos_theta = (array_a @ array_b) / (norm(array_a) * norm(array_b))

    # A cos theta between -1 and -0.7 should correspond to antiparallel vectors,
    # with a tolerance of about 45 degrees to either side.
    return cos_theta < -0.7


def detect_connection(a: Vector, b: Vector):
    return approximately_equal(a[0], b[0]) and are_antiparallel(a, b)


def detect_connections(indexed_curves):
    connected_curves = indexed_curves

    # Loop through each possible permutation of A and B pairs in the curves
    for a, b in combinations(connected_curves, 2):
        # Given a list [0, 1, 2, 3, 4] then [:2] gives [0, 1] and [-1:-3:-1] gives [4, 3]
        # Create two lists with both the start and the end points of A and B. Note the shift in nomenclature,
        # 'points' and 'at' refers to segment connection points, and 'vec' refers to the bezier points.
        a_points = [
            {"at": "begin", "vec": a["points"][:2]},
            {"at": "end", "vec": a["points"][-1:-3:-1]},
        ]
        b_points = [
            {"at": "begin", "vec": b["points"][:2]},
            {"at": "end", "vec": b["points"][-1:-3:-1]},
        ]

        # For both points of a and b, check each cartesian product.
        for a_point, b_point in product(a_points, b_points):
            if detect_connection(a_point["vec"], b_point["vec"]):
                a["connections"].append(
                    {
                        "at": a_point["at"],
                        "other_id": b["index"],
                        "other_at": b_point["at"],
                    }
                )
                b["connections"].append(
                    {
                        "at": b_point["at"],
                        "other_id": a["index"],
                        "other_at": a_point["at"],
                    }
                )

    return connected_curves


def convert_to_message(connected_curves):
    section = world_pb2.Section()
    for curve in connected_curves:
        segment = section.rails.segments.add()
        segment.id.value = curve["index"]

        for point in curve["points"]:
            point_out = segment.points.add()
            point_out.x = point[0]
            point_out.y = point[1]
            point_out.z = point[2]

        for connection in curve["connections"]:
            if connection["at"] not in ["begin", "end"]:
                raise ValueError(
                    f"connection 'at' has unexpected value: {connection['at']}"
                )
            if connection["other_at"] not in ["begin", "end"]:
                raise ValueError(
                    f"connection 'other_at' has unexpected value: {connection['other_at']}"
                )

            connection_message = (
                segment.connections_at_begin_point.add()
                if (connection["at"] == "begin")
                else segment.connections_at_end_point.add()
            )
            connection_message.id.value = connection["other_id"]
            connection_message.connection_point = (
                world_pb2.ConnectionPoint.CONNECTION_POINT_BEGIN
                if connection["other_at"] == "begin"
                else world_pb2.ConnectionPoint.CONNECTION_POINT_END
            )

    return section


def convert(curves):
    # covert the array of curves into a list of dicts containing index, points, and connections
    indexed_curves = [
        {"index": index, "points": points, "connections": []}
        for index, points in enumerate(curves, 1)
    ]

    # Find all connections between each curve segment
    connected_curves = detect_connections(indexed_curves)

    # Convert to proto message
    return convert_to_message(connected_curves)
