# bmd structure
#
#           _
# 4 char: b, m, d, \0
#
# 1 int: faceCount
# 1 int: texCount
#
# texCount times =>
#   512 char: texName
#
# faceCount times =>
# 1 int: texture num
#   3 times =>
#     3 float: vertex coordinates
#     3 float: vertex normals
#     2 float: uv coordinates

import bpy
import struct

def write_bmd(context, filepath):
    print("running write_bmd...")

    #prepare axes
    xx = 0
    yx = 2
    zx = 1

    xm = +1
    ym = +1
    zm = -1

    #test for valid selection
    ob = bpy.context.selected_objects[0]

    #convert to triangles
    if bpy.context.mode == 'EDIT_MESH':
        bpy.ops.object.editmode_toggle()
    bpy.ops.object.editmode_toggle()
    bpy.ops.mesh.quads_convert_to_tris()
    bpy.ops.object.editmode_toggle()

    #test for mesh
    if ob.type != 'MESH':
        print( 'Error: Not a Mesh!' )
        return {'FAILURE'}

    #open file
    file = open(filepath, 'w+b')

    #Magic number
    file.write( struct.pack( 'cccc',
        bytes( 'b', 'ascii' ),
        bytes( 'm', 'ascii' ),
        bytes( 'd', 'ascii' ),
        bytes( '\0', 'ascii' ) ) )

    #Write object data
    file.write( struct.pack( 'ii', len( ob.data.faces ), len( ob.data.uv_textures ) ) )

    texNums = {}

    #512 byte texture names
    for texture in ob.data.uv_textures:
        path = ob.data.uv_textures[0].data[0].image.filepath

        #save texture number
        texNums[ob.data.uv_textures[0].data[0].image] = len( texNums )

        length = len( path )
        for byte in range( 0, length ):
            file.write( struct.pack( 'c', bytes( path[byte], 'ascii' ) ) )
        for byte in range( length, 512 ):
            file.write( struct.pack( 'c', bytes( '\0', 'ascii' ) ) )

    #geometry data
    for faceNum in range( 0, len( ob.data.faces ) ):
        file.write( struct.pack( 'i', texNums[ob.data.uv_textures[0].data[0].image] ) )
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

    file.close()
    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty


class ExportBMD(bpy.types.Operator, ExportHelper):
    '''This appears in the tooltip of the operator and in the generated docs.'''
    bl_idname = "export.bmd"  # this is important since its how bpy.ops.export.some_data is constructed
    bl_label = "Export Binary Model Data (BMD)"

    # ExportHelper mixin class uses this
    filename_ext = ".bmd"

    filter_glob = StringProperty(
            default="*.bmd",
            options={'HIDDEN'},
            )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    #use_setting = BoolProperty(
    #        name="Example Boolean",
    #        description="Example Tooltip",
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
    #bpy.ops.export.bmd('INVOKE_DEFAULT')
