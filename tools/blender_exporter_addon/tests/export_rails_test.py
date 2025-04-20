#  SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
#  SPDX-License-Identifier: MIT
from ubahn_exporter_blender_addon.export_rails import convert
from ubahn_exporter_blender_addon.world_pb2 import ConnectionPoint


def test_convert_simple():
    """Test conversion of two connected straight lines"""
    curves = [[[0.0, -1.0, 0.0], [0.0, 0.0, 0.0]], [[0.0, 0.0, 0.0], [0.0, 1.0, 0.0]]]

    section = convert(curves)
    assert len(section.rails.segments) == 2
    segment_1 = section.rails.segments[0]
    assert segment_1.id.value == 1
    assert len(segment_1.connections_at_begin_point) == 0
    assert len(segment_1.connections_at_end_point) == 1
    assert segment_1.connections_at_end_point[0].id.value == 2
    assert (
        segment_1.connections_at_end_point[0].connection_point
        == ConnectionPoint.CONNECTION_POINT_BEGIN
    )

    segment_2 = section.rails.segments[1]
    assert segment_2.id.value == 2
    assert len(segment_2.connections_at_begin_point) == 1
    assert len(segment_2.connections_at_end_point) == 0
    assert segment_2.connections_at_begin_point[0].id.value == 1
    assert (
        segment_2.connections_at_begin_point[0].connection_point
        == ConnectionPoint.CONNECTION_POINT_END
    )
