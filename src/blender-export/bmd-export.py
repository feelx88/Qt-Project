# bmd structure
#
#           _
# 4 char: b, m, d, \0
#
# 1 int: faceCount
# 1 int: texCount
#
# 512 char: texName
#
# faceCount times =>
#   3 times =>
#     1 int: 0 to keep backward compatibiity
#     3 float: vertex coordinates
#     3 float: vertex normals
#     2 float: uv coordinates

bl_info = {
    "name": "BMD Exporter",
    "description": "Export Mesh to Binary Mesh Data.",
    "author": "feelx88",
    "version": (1, 0),
    "blender": (2, 6, 0),
    "location": "File > Export",
    "category": "Import-Export"}

import bpy
import struct
import os.path
import shutil

def write_bmd(context, filepath):
    print("running write_bmd...")

    path = os.path.dirname( filepath )

    #prepare axes
    xx = 0
    yx = 2
    zx = 1

    xm = +1
    ym = +1
    zm = -1

    if len( bpy.context.selected_objects ) != 1:
        return {'CANCELLED'}

    ob = bpy.context.selected_objects[0]

    #test for mesh
    if ob.type != 'MESH':
        print( 'Error: Not a Mesh!' )
        return {'FINISHED'}

    bpy.ops.object.mode_set( mode = 'OBJECT' )

    bpy.ops.object.duplicate()
    ob.select = False

    ob = bpy.context.selected_objects[0]

    #convert to triangles
    bpy.ops.object.mode_set( mode = 'EDIT' )
    bpy.ops.mesh.quads_convert_to_tris()
    bpy.ops.object.mode_set( mode = 'OBJECT' )

    #open file
    file = open(filepath, 'w+b')

    #Magic number
    file.write( struct.pack( 'cccc',
        bytes( 'b', 'ascii' ),
        bytes( 'm', 'ascii' ),
        bytes( 'd', 'ascii' ),
        bytes( '\0', 'ascii' ) ) )

    #Write object data
    file.write( struct.pack( 'ii', len( ob.data.faces ), 1 ) )

    #512 byte texture name
    tex = ob.data.uv_textures[0].data[0].image
    texFile = os.path.basename( tex.filepath )
    tex.file_format = 'PNG'

    if texFile == '' or not os.path.exists( path + os.path.sep + texFile ):
        bpy.context.scene.render.image_settings.file_format = 'PNG'
        bpy.context.scene.render.image_settings.color_mode = 'RGBA'
        texFile = tex.name + '.png'
        tex.save_render( filepath = path + os.path.sep + texFile )

    texLen = len( texFile )
    for byte in range( 0, texLen ):
        file.write( struct.pack( 'c', bytes( texFile[byte], 'ascii' ) ) )
    for byte in range( texLen, 512 ):
        file.write( struct.pack( 'c', bytes( '\0', 'ascii' ) ) )

    #geometry data
    for faceNum in range( 0, len( ob.data.faces ) ):
        file.write( struct.pack( 'i', 0 ) ) #for compatibility
        for x in range(0,3):
            vertex = ob.data.vertices[ob.data.faces[faceNum].vertices[x]]
            file.write( struct.pack( 'fff',
                vertex.co[xx] * xm,
                vertex.co[yx] * ym,
                vertex.co[zx] * zm ) )
            file.write( struct.pack( 'fff',
                vertex.normal[xx] * xm,
                vertex.normal[yx] * ym,
                vertex.normal[zx] * zm ) )

            uvdata = ob.data.uv_textures[0].data[faceNum].uv[x]
            file.write( struct.pack( 'ff', uvdata[0], 1 - uvdata[1] ) )
t
    file.close()
    bpy.ops.object.delete()
    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty


class ExportBMD(bpy.types.Operator, ExportHelper):
    '''This appears in the tooltip of the operator and in the generated docs.'''
    bl_idname = "export_mesh.bmd"  # this is important since its how bpy.ops.export.some_data is constructed
    bl_label = "Export Binary Mesh Data (BMD)"

    # ExportHelper mixin class uses this
    filename_ext = ".bmd"

    filter_glob = StringProperty(
            default="*.bmd",
            options={'HIDDEN'},
            )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    #onlySelected = BoolProperty(
    #        name="Export only selected",
    #        description="Exports only the selected object.",
    #        default=True,
    #        )

    #type = EnumProperty(
    #        name="Example Enum",
    #        description="Choose between two items",
    #        items=(('OPT_A', "First Option", "Description one"),
    #               ('OPT_B', "Second Option", "Description two")),
    #        default='OPT_A',
    #        )

    @classmethod
    def poll(cls, context):
        return context.active_object is not None

    def execute(self, context):
        return write_bmd(context, self.filepath)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportBMD.bl_idname, text="BMD Export")


def register():
    bpy.utils.register_class(ExportBMD)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    try:
        bpy.utils.unregister_class(ExportBMD)
        bpy.types.INFO_MT_file_export.remove(menu_func_export)
    except:
        pass

if __name__ == "__main__":
    register()
    #bpy.ops.export_mesh.bmd('INVOKE_DEFAULT')
