import bpy
import struct

def write_bmd(context, filepath):
    print("running write_bmd...")
    
    #test for valid selection
    object = bpy.context.selected_objects[0]
    
    #convert to triangles
    bpy.ops.object.editmode_toggle()
    bpy.ops.mesh.quads_convert_to_tris()
    bpy.ops.object.editmode_toggle()
    
    #test for mesh
    if object.type != 'MESH':
        print( 'Error: Not a Mesh!' )
        return {'FAILURE'}
    
    #open file
    file = open(filepath, 'w+b')
    
    #Magic number
    file.write( struct.pack( 'ii', 42, 11 ) )
    
    #Write object data
    file.write( struct.pack( 'ii', len( object.data.faces ) * 3, len( object.data.uv_textures ) ) )
    
    #512 byte texture names
    for texture in object.data.uv_textures:
        path = object.data.uv_textures[0].data[0].image.filepath
        length = len( path )
        for byte in range( 0, length ):
            file.write( struct.pack( 'c', bytes( path[byte], 'ascii' ) ) )
        for byte in range( length, 512 ):
            file.write( struct.pack( 'c', bytes( '\0', 'ascii' ) ) )
    
    #geometry data
    for face in object.data.faces:
        for index in face.vertices:
            vertex = object.data.vertices[index]
            file.write( struct.pack( 'fff', vertex.co[0], vertex.co[1], vertex.co[2] ) )
            file.write( struct.pack( 'fff', vertex.normal[0], vertex.normal[1], vertex.normal[2] ) )
            
            for texture in object.data.uv_textures:
                texdata = texture.data[index]
                file.write( struct.pack( 'ff', texdata.uv1[0], texdata.uv1[1] ) )

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
    #bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    try:
        bpy.utils.unregister_class(ExportBMD)
        #bpy.types.INFO_MT_file_export.remove(menu_func_export)
    except:
        pass

if __name__ == "__main__":
    register()
    bpy.ops.export.bmd('INVOKE_DEFAULT')