# Export level data from blender:
# - Meshes used for level geometry
# - Paths used for enemies
# - Markers used for player's starting position, direction changes etc.

bl_info = {
    "name": "Level Exporter",
    "description": "Export Level.",
    "author": "feelx88",
    "version": (1, 0),
    "blender": (2, 6, 0),
    "location": "File > Export",
    "category": "Import-Export"}

import bpy
import struct

import os.path
import xml.dom.minidom

def appendElement( self, document, nodeName ):
    node = document.createElement( nodeName )
    self.appendChild( node )
    return node

def appendTextElement( self, document, nodeName, text ):
    node = document.createElement( nodeName )
    text = document.createTextNode( text )
    node.appendChild( text )
    self.appendChild( node )
    return node

def write_leveldata(context, filepath):
    print("running write_leveldata...")

    #prepare axes
    xx = 0
    yx = 2
    zx = 1

    xm = +1
    ym = +1
    zm = -1

    #activate object mode and deselect all objects
    bpy.ops.object.mode_set( mode = 'OBJECT' )
    bpy.ops.object.select_all( action = 'DESELECT' )

    #apend convenience methods
    setattr( xml.dom.minidom.Element, 'appendElement', appendElement )
    setattr( xml.dom.minidom.Element, 'appendTextElement', appendTextElement )

    #create xml document
    doc = xml.dom.minidom.Document()
    rootElem = doc.createElement( 'Level' )
    doc.appendChild( rootElem )

    directory = os.path.dirname( filepath ) + '/'

    #add data to dom
    for ob in bpy.data.objects:
        if ob.type == 'MESH':
            if 'player' in ob.keys():
                #player start
                elem = rootElem.appendElement( doc, 'Player' )
                elem.appendTextElement( doc, 'X', str( ob.location[xx] * xm ) )
                elem.appendTextElement( doc, 'Y', str( ob.location[yx] * ym ) )
                elem.appendTextElement( doc, 'Z', str( ob.location[zx] * zm ) )
            else:
                #level geometry
                elem = rootElem.appendElement( doc, 'Mesh' )
                elem.appendTextElement( doc, 'File', ob.name + '.bmd' )
                ob.select = True
                bpy.ops.export_mesh.bmd( filepath = directory + ob.name + '.bmd' )
                ob.select = False
                elem.appendTextElement( doc, 'X', str( ob.location[xx] * xm ) )
                elem.appendTextElement( doc, 'Y', str( ob.location[yx] * ym ) )
                elem.appendTextElement( doc, 'Z', str( ob.location[zx] * zm ) )
        elif ob.type == 'CURVE':
            if ob.data.splines[0].type == 'BEZIER' and 'enemy' in ob.keys():
                #enemy paths
                elem = rootElem.appendElement( doc, 'Enemy' )
                elem.appendTextElement( doc, 'Name', ob.id_data['enemy'] )
                numNodes = len( ob.data.splines[0].bezier_points )
                if numNodes % 2 != 0:
                    numNodes -= 1
                for index in range( 0, numNodes ):
                    node = ob.data.splines[0].bezier_points[index]
                    point = elem.appendElement( doc, 'PathNode' )
                    point.appendTextElement( doc, 'X', str( node.co[xx] * xm ) )
                    point.appendTextElement( doc, 'Y', str( node.co[yx] * ym ) )
                    point.appendTextElement( doc, 'Z', str( node.co[zx] * zm ) )
                    point = elem.appendElement( doc, 'PathNode' )
                    point.appendTextElement( doc, 'X', str( node.handle_right[xx] * xm ) )
                    point.appendTextElement( doc, 'Y', str( node.handle_right[yx] * ym ) )
                    point.appendTextElement( doc, 'Z', str( node.handle_right[zx] * zm ) )
                    node = ob.data.splines[0].bezier_points[index + 1]
                    point = elem.appendElement( doc, 'PathNode' )
                    point.appendTextElement( doc, 'X', str( node.handle_left[xx] * xm ) )
                    point.appendTextElement( doc, 'Y', str( node.handle_left[yx] * ym ) )
                    point.appendTextElement( doc, 'Z', str( node.handle_left[zx] * zm ) )
                    point = elem.appendElement( doc, 'PathNode' )
                    point.appendTextElement( doc, 'X', str( node.co[xx] * xm ) )
                    point.appendTextElement( doc, 'Y', str( node.co[yx] * ym ) )
                    point.appendTextElement( doc, 'Z', str( node.co[zx] * zm ) )

    #write xml
    file = open( filepath, 'w+' )
    #file.write( doc.toprettyxml() )
    file.write( doc.toxml() )
    file.close()
    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty


class ExportBMD(bpy.types.Operator, ExportHelper):
    '''This appears in the tooltip of the operator and in the generated docs.'''
    bl_idname = "export.leveldata"  # this is important since its how bpy.ops.export.some_data is constructed
    bl_label = "Export Level Data (XML)"

    # ExportHelper mixin class uses this
    filename_ext = ".xml"

    filter_glob = StringProperty(
            default="*.xml",
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
        return write_leveldata(context, self.filepath)


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
    bpy.ops.export.leveldata('INVOKE_DEFAULT')
