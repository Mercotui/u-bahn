#  SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
#  SPDX-License-Identifier: MIT
from itertools import pairwise

import bpy
from bpy.props import BoolProperty, EnumProperty, StringProperty
from bpy.types import Operator
from bpy_extras.io_utils import ExportHelper

from . import export_rails


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
        curves = []
        for obj in bpy.data.objects:
            if obj.name == "ubahn-rails":
                if obj.type != "CURVE":
                    self.report(
                        {"ERROR"},
                        "ubahn-rails object is not a curve, instead type: {}".format(
                            obj.type
                        ),
                    )
                    continue

                for spline in obj.data.splines:
                    if spline.type != "BEZIER":
                        self.report(
                            {"ERROR"},
                            "ubahn-rails spline is not a bezier curve, instead: {}".format(
                                obj.type
                            ),
                        )
                        continue

                    # Iterate over each pair of control points
                    for a, b in pairwise(spline.bezier_points):
                        # Get the control points of the cubic Bézier curve
                        points = [a.co, a.handle_right, b.handle_left, b.co]

                        # Store the Bézier curve as a generic array of arrays of coordinates
                        curves.append([[point.x, point.y, point.z] for point in points])

                    # Check if we need to complete the loop of this spline
                    if spline.use_cyclic_u:
                        # Get the last and first control points of the cubic Bézier curve
                        a = spline.bezier_points[-1]
                        b = spline.bezier_points[0]
                        points = [a.co, a.handle_right, b.handle_left, b.co]
                        curves.append([[point.x, point.y, point.z] for point in points])

        section = export_rails.convert(curves)

        f = open(self.filepath, "wb")
        f.write(export_rails.serialize(section))
        f.close()
        return {"FINISHED"}


def menu_func_export(self, context):
    self.layout.operator(ExportUBahnSection.bl_idname, text="Export U-Bahn Section")


def register():
    bpy.utils.register_class(ExportUBahnSection)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportUBahnSection)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)
