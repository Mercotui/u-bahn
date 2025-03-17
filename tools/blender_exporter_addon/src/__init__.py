#  SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
#  SPDX-License-Identifier: MIT
import bpy
from bpy.props import BoolProperty, EnumProperty, StringProperty
from bpy.types import Operator
from bpy_extras.io_utils import ExportHelper

# According to the internet lore, relative imports should work?
# https://blender.stackexchange.com/a/202672
from . import world_pb2


def write_some_data(context, filepath, use_some_setting):
    point1 = world_pb2.Point()
    point1.x = 20
    point1.y = 0
    point2 = world_pb2.Point()
    point2.x = 0
    point2.y = 0
    point3 = world_pb2.Point()
    point3.x = 0
    point3.y = 20
    point4 = world_pb2.Point()
    point4.x = 20
    point4.y = 20

    section = world_pb2.Section()

    segment1 = section.rails.segments.add()
    segment1.id.value = 1
    segment1.points.append(point1)
    segment1.points.append(point2)
    segment1.connections_at_begin_point.add().id.value = 4

    segment2 = section.rails.segments.add()
    segment2.id.value = 2
    segment2.points.append(point2)
    segment2.points.append(point3)
    segment2.connections_at_begin_point.add().id.value = 1

    segment3 = section.rails.segments.add()
    segment3.id.value = 3
    segment3.points.append(point3)
    segment3.points.append(point4)
    segment3.connections_at_begin_point.add().id.value = 2

    segment4 = section.rails.segments.add()
    segment4.id.value = 4
    segment4.points.append(point4)
    segment4.points.append(point1)
    segment4.connections_at_begin_point.add().id.value = 3

    print("running write_some_data...")
    f = open(filepath, "wb")
    f.write(section.SerializeToString())
    f.close()
    return {"FINISHED"}


class ExportUBahnSection(Operator, ExportHelper):
    """Export a world section for the U-Bahn game"""

    bl_idname = "exportubahn.export"
    bl_label = "Export U-Bahn Section"

    filename_ext = ".bin"

    filter_glob: StringProperty(
        default="*.bin",
        options={"HIDDEN"},
        maxlen=255,
    )

    use_setting: BoolProperty(
        name="Example Boolean",
        description="Example Tooltip",
        default=True,
    )

    type: EnumProperty(
        name="Example Enum",
        description="Choose between two items",
        items=(
            ("OPT_A", "First Option", "Description one"),
            ("OPT_B", "Second Option", "Description two"),
        ),
        default="OPT_A",
    )

    def execute(self, context):
        return write_some_data(context, self.filepath, self.use_setting)


def menu_func_export(self, context):
    self.layout.operator(ExportUBahnSection.bl_idname, text="Export U-Bahn Section")


def register():
    bpy.utils.register_class(ExportUBahnSection)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportUBahnSection)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.exportubahn.export("INVOKE_DEFAULT")
