#  SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
#  SPDX-License-Identifier: MIT

from . import world_pb2


def serialize(section):
    return section.SerializeToString()


def convert(curves):
    section = world_pb2.Section()

    for curve_index, curve in enumerate(curves, 1):
        segment = section.rails.segments.add()
        segment.id.value = curve_index

        for point in curve:
            point_out = segment.points.add()
            point_out.x = point[0]
            point_out.y = point[1]
            point_out.z = point[2]
        # TODO(Menno 18.04.2025) Still need to detect connections
        # segment.connections_at_begin_point.add().id.value = 4

    return section
