import bpy
import struct

def write_bmd(context, filepath):
    print("running write_bmd...")
    file = open(filepath, 'w+b')
    
    #Magic number
    file.write( struct.pack( 'ii', 42, 11 ) )
    
    #Header
    numObjects = 0
    for object in bpy.data.objects:
        if object.type == 'MESH':
            numObjects++
            
    file.write( struct.pack( 'i', numObjects ) )
    
    #Iterate over all objects
    for object in bpy.data.objects:
        if object.type == 'MESH':
            file.write( struct.pack( 'i', len( object.data.faces ) ) )
            for face in object.data.faces:
                for index in face.vertices:
                    vertex = object.data.vertices[index]
                    file.write( struct.pack( 'fff', vertex.co[0], vertex.co[1], vertex.co[2] ) )
                    file.write( struct.pack( 'fff', vertex.normal[0], vertex.normal[1], vertex.normal[2] ) )

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