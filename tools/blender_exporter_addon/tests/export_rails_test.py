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


def test_connection_proximity():
    # Tracks share an exact vertex, should connect
    curves_0 = [[[0, -1.0, 0], [0, 0, 0]], [[0, 0, 0], [0, 1.0, 0]]]
    section_0 = convert(curves_0)
    assert len(section_0.rails.segments[0].connections_at_end_point) == 1
    assert len(section_0.rails.segments[1].connections_at_begin_point) == 1

    # Track end vertices are 25 centimeters apart, should still connect
    curves_25 = [[[0, -1.0, 0], [0, 0, 0]], [[0, 0.25, 0], [0, 1.0, 0]]]
    section_25 = convert(curves_25)
    assert len(section_25.rails.segments[0].connections_at_end_point) == 1
    assert len(section_25.rails.segments[1].connections_at_begin_point) == 1

    # Vertices are 40 centimeters apart, which is too much to be considered a connection
    curves_40 = [[[0, -1.0, 0], [0, 0, 0]], [[0, 0.4, 0], [0, 1.0, 0]]]
    section_40 = convert(curves_40)
    assert len(section_40.rails.segments[0].connections_at_end_point) == 0
    assert len(section_40.rails.segments[1].connections_at_begin_point) == 0


def test_connection_angle():
    # Tracks aligned with 0 degree kink, should connect
    curves_0 = [[[0, -1.0, 0], [0, 0, 0]], [[0, 0, 0], [0, 1.0, 0]]]
    section_0 = convert(curves_0)
    assert len(section_0.rails.segments[0].connections_at_end_point) == 1
    assert len(section_0.rails.segments[1].connections_at_begin_point) == 1

    # A 40 degree kink should connect
    curves_40 = [[[0, -1.0, 0], [0, 0, 0]], [[0, 0, 0], [0.8391, 1.0, 0]]]
    section_40 = convert(curves_40)
    assert len(section_40.rails.segments[0].connections_at_end_point) == 1
    assert len(section_40.rails.segments[1].connections_at_begin_point) == 1

    # 50 degrees is too kinky, should not connect
    curves_50 = [[[0, -1.0, 0], [0, 0, 0]], [[0, 0, 0], [1.1918, 1.0, 0]]]
    section_50 = convert(curves_50)
    assert len(section_50.rails.segments[0].connections_at_end_point) == 0
    assert len(section_50.rails.segments[1].connections_at_begin_point) == 0

    # 130 degrees is just freaky, should not connect
    curves_130 = [[[0, -1.0, 0], [0, 0, 0]], [[0, 0, 0], [0.8391, -1.0, 0]]]
    section_130 = convert(curves_130)
    assert len(section_130.rails.segments[0].connections_at_end_point) == 0
    assert len(section_130.rails.segments[1].connections_at_begin_point) == 0


def test_convert_dogbone():
    """Test a dogbone shaped track that has two turning loops on either end"""
    curves = [
        # Dogbone left loop
        [[-7.0, 0, 0], [-16.0, 0, 0], [-11.0, -3.0, 0], [-16.0, -6.0, 0]],
        [[-16.0, -6.0, 0], [-23.0, -11.0, 0], [-22.0, -4.0, 0], [-22.0, 0, 0]],
        [[-22.0, 0, 0], [-22.0, 2.0, 0], [-26.0, 9.0, 0], [-17.0, 9.0, 0]],
        [[-17.0, 9.0, 0], [-13.0, 8.0, 0], [-15.0, 0, 0], [-7.0, 0, 0]],
        # This is the middle section of the dogbone
        [[-7.0, 0, 0], [0, 0, 0], [1.0, 0, 0], [8.0, 0, 0]],
        # Dogbone right loop
        [[8.0, 0, 0], [20, 0, 0], [11.0, 7.0, 0], [18.0, 7.0, 0]],
        [[18.0, 7.0, 0], [22.0, 7.0, 0], [25.0, 7.0, 0], [25.0, 0, 0]],
        [[25.0, 0, 0], [25.0, -6.0, 0], [27.0, -8.0, 0], [20, -8.0, 0]],
        [[20, -8.0, 0], [15.0, -8.0, 0], [16.0, 0, 0], [8.0, 0, 0]],
    ]

    section = convert(curves)
    assert len(section.rails.segments) == 9
    # Each segment should have one connection on either end, except for the middle segment #4
    for i in [0, 1, 2, 3, 5, 6, 7, 8]:
        assert (
            len(section.rails.segments[i].connections_at_begin_point) == 1
        ), f"where i={i}"
        assert (
            len(section.rails.segments[i].connections_at_end_point) == 1
        ), f"where i={i}"

    # The middle segment has two connections on either side
    assert len(section.rails.segments[4].connections_at_begin_point) == 2
    assert len(section.rails.segments[4].connections_at_end_point) == 2
