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
from mathutils import Vector
import re

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

#def switchToObjectMode():
#    if bpy.context.object

def write_leveldata(context, filepath, overwrite_existing, clean):
    print("running write_leveldata...")

    #prepare axes
    xx = 0
    yx = 2
    zx = 1

    xm = +1
    ym = +1
    zm = -1

    #activate object mode and deselect all objects
    if bpy.ops.object.mode_set.poll():
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

    if clean:
        for file in os.listdir( directory ):
            if re.search( '.*\.bmd$|.*.\.xml$|.*\.png$', file ) != None:
                os.remove( directory + file )

    #add data to dom
    for ob in bpy.context.scene.objects:
        if ob.type == 'MESH':
            if 'noexport' in ob.keys():
                continue
            elif 'player' in ob.keys():
                #player start
                elem = rootElem.appendElement( doc, 'Player' )
                pos = elem.appendElement( doc, 'StartPosition' )
                pos.appendTextElement( doc, 'X', str( ob.location[xx] * xm ) )
                pos.appendTextElement( doc, 'Y', str( ob.location[yx] * ym ) )
                pos.appendTextElement( doc, 'Z', str( ob.location[zx] * zm ) )
            elif 'direction' in ob.keys():
                elem = rootElem.appendElement( doc, 'DirectionChanger' )
                pos = elem.appendElement( doc, 'Position' )
                pos.appendTextElement( doc, 'X', str( ob.location[xx] * xm ) )
                pos.appendTextElement( doc, 'Y', str( ob.location[yx] * ym ) )
                pos.appendTextElement( doc, 'Z', str( ob.location[zx] * zm ) )

                d = Vector()
                d[xx] = 0;
                d[yx] = 0;
                d[zx] = 1;
                d.rotate( ob.rotation_euler )

                direction = elem.appendElement( doc, 'Direction' )
                direction.appendTextElement( doc, 'X', str( d[xx] * xm ) )
                direction.appendTextElement( doc, 'Y', str( d[yx] * ym ) )
                direction.appendTextElement( doc, 'Z', str( d[zx] * zm ) )

                elem.appendTextElement( doc, 'Radius', str( ob.dimensions[0] / 2 ) )

                mode = 'Fixed'
                if ob.id_data['direction'] == 'free':
                    mode = 'Free'

                elem.appendTextElement( doc, 'Mode', mode )
            else:
                #level geometry
                name = ob.name
                if len( ob.users_group ) > 0:
                    if( ob.users_group[0].name == 'NoCollision' ):
                        if len( ob.users_group ) > 1:
                            name = ob.users_group[1].name
                    else:
                        name = ob.users_group[0].name

                if overwrite_existing or not os.path.exists( directory + name + '.bmd' ):
                    bpy.ops.object.select_name( name = ob.name )
                    bpy.ops.object.mode_set( mode = 'OBJECT' )
                    bpy.ops.export_mesh.bmd( filepath = directory + name + '.bmd' )
                    bpy.ops.object.select_all( action = 'DESELECT' )

                elem = rootElem.appendElement( doc, 'Mesh' )
                elem.appendTextElement( doc, 'File', name + '.bmd' )

                pos = elem.appendElement( doc, 'Position' )
                pos.appendTextElement( doc, 'X', str( ob.location[xx] * xm ) )
                pos.appendTextElement( doc, 'Y', str( ob.location[yx] * ym ) )
                pos.appendTextElement( doc, 'Z', str( ob.location[zx] * zm ) )

                rot = elem.appendElement( doc, 'Rotation' )
                rot.appendTextElement( doc, 'X', str( ob.rotation_euler[xx] * xm ) )
                rot.appendTextElement( doc, 'Y', str( ob.rotation_euler[yx] * ym ) )
                rot.appendTextElement( doc, 'Z', str( ob.rotation_euler[zx] * zm ) )

                noCollision = 'False'

                for group in ob.users_group:
                    if group.name == 'NoCollision':
                        noCollision = 'True'
                elem.appendTextElement( doc, 'NoCollision', noCollision )
        elif ob.type == 'CURVE':
            if ob.data.splines[0].type == 'BEZIER' and 'enemy' in ob.keys():
                #enemy paths
                elem = rootElem.appendElement( doc, 'Enemy' )
                elem.appendTextElement( doc, 'Name', ob.id_data['enemy'] )

                hp = 3
                if 'hitpoints' in ob.keys():
                    hp = ob.id_data['hitpoints']

                elem.appendTextElement( doc, 'Hitpoints', str( hp ) )
                elem.appendTextElement( doc, 'Looping', str( ob.data.splines[0].use_cyclic_u ) )
                numNodes = len( ob.data.splines[0].bezier_points ) - 1

                for index in range( 0, numNodes ):
                    node = ob.data.splines[0].bezier_points[index]
                    nx = ob.location[xx] * xm
                    ny = ob.location[yx] * ym
                    nz = ob.location[zx] * zm
                    point = elem.appendElement( doc, 'PathNode' )
                    point.appendTextElement( doc, 'X', str( node.co[xx] * xm + nx ) )
                    point.appendTextElement( doc, 'Y', str( node.co[yx] * ym + ny ) )
                    point.appendTextElement( doc, 'Z', str( node.co[zx] * zm + nz) )
                    point = elem.appendElement( doc, 'PathNode' )
                    point.appendTextElement( doc, 'X', str( node.handle_right[xx] * xm + nx ) )
                    point.appendTextElement( doc, 'Y', str( node.handle_right[yx] * ym + ny ) )
                    point.appendTextElement( doc, 'Z', str( node.handle_right[zx] * zm + nz ) )
                    node = ob.data.splines[0].bezier_points[index + 1]
                    point = elem.appendElement( doc, 'PathNode' )
                    point.appendTextElement( doc, 'X', str( node.handle_left[xx] * xm + nx ) )
                    point.appendTextElement( doc, 'Y', str( node.handle_left[yx] * ym + ny ) )
                    point.appendTextElement( doc, 'Z', str( node.handle_left[zx] * zm + nz ) )
                    point = elem.appendElement( doc, 'PathNode' )
                    point.appendTextElement( doc, 'X', str( node.co[xx] * xm + nx ) )
                    point.appendTextElement( doc, 'Y', str( node.co[yx] * ym + ny ) )
                    point.appendTextElement( doc, 'Z', str( node.co[zx] * zm + nz ) )

    #write xml
    file = open( filepath, 'w+' )
    file.write( doc.toprettyxml() )
    #file.write( doc.toxml() )
    file.close()
    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty


class ExportLevel(bpy.types.Operator, ExportHelper):
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
    overwrite_existing = BoolProperty(
            name="Owerwrite existing bmd files",
            description="",
            default=True,
            )
    clean = BoolProperty(
            name="Clean directory",
            description="Remove all .bmd, .xml and .png files from directory",
            default=False,
            )

    #type = EnumProperty(
    #        name="Example Enum",
    #        description="Choose between two items",
    #        items=(('OPT_A', "First Option", "Description one"),
    #               ('OPT_B', "Second Option", "Description two")),
    #        default='OPT_A',
    #        )

    @classmethod
    def poll(cls, context):
        return True#context.active_object is not None

    def execute(self, context):
        return write_leveldata(context, self.filepath, self.overwrite_existing, self.clean)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportLevel.bl_idname, text="Level Export")


def register():
    bpy.utils.register_class(ExportLevel)
    #bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    try:
        bpy.utils.unregister_class(ExportLevel)
        #bpy.types.INFO_MT_file_export.remove(menu_func_export)
    except:
        pass

if __name__ == "__main__":
    register()
    bpy.ops.export.leveldata('INVOKE_DEFAULT')