# Kratos Imports
import KratosMultiphysics
import KratosMultiphysics.MeshMovingApplication as MeshMovingApplication
import KratosMultiphysics.KratosUnittest as KratosUnittest


def GenerateModelPart():
    model = KratosMultiphysics.Model()
    model_part = model.CreateModelPart("Main")

    model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DISPLACEMENT)
    model_part.AddNodalSolutionStepVariable(KratosMultiphysics.MESH_DISPLACEMENT)

    domain = KratosMultiphysics.Quadrilateral2D4(
        KratosMultiphysics.Node(1, 0.0, 0.0, 0.0),
        KratosMultiphysics.Node(2, 0.0, 2.0, 0.0),
        KratosMultiphysics.Node(3, 2.0, 2.0, 0.0),
        KratosMultiphysics.Node(4, 2.0, 0.0, 0.0)
    )

    parameters = KratosMultiphysics.Parameters("""{
        "element_name" : "Element2D3N",
        "number_of_divisions" : 2,
        "create_skin_sub_model_part" : false
    }""")

    KratosMultiphysics.StructuredMeshGeneratorProcess(
        domain,
        model_part,
        parameters
    ).Execute()

    return model_part


class TestImposeMeshMotionProcess(KratosUnittest.TestCase):

    def CheckNodes(self, modelPart: KratosMultiphysics.ModelPart):
        self.assertEqual(len(modelPart.Nodes), 9)

        self.assertVectorAlmostEqual(modelPart.Nodes[0].GetValue(KratosMultiphysics.MESH_DISPLACEMENT), [0.0, 2.0, 4.0])
        self.assertVectorAlmostEqual(modelPart.Nodes[1].GetValue(KratosMultiphysics.MESH_DISPLACEMENT), [0.0, 2.0, 4.0])
        self.assertVectorAlmostEqual(modelPart.Nodes[2].GetValue(KratosMultiphysics.MESH_DISPLACEMENT), [0.0, 2.0, 4.0])
        self.assertVectorAlmostEqual(modelPart.Nodes[3].GetValue(KratosMultiphysics.MESH_DISPLACEMENT), [-1.0, 2.0, 4.0])
        self.assertVectorAlmostEqual(modelPart.Nodes[4].GetValue(KratosMultiphysics.MESH_DISPLACEMENT), [-1.0, 2.0, 4.0])
        self.assertVectorAlmostEqual(modelPart.Nodes[5].GetValue(KratosMultiphysics.MESH_DISPLACEMENT), [-1.0, 2.0, 4.0])
        self.assertVectorAlmostEqual(modelPart.Nodes[6].GetValue(KratosMultiphysics.MESH_DISPLACEMENT), [-2.0, 2.0, 6.0])
        self.assertVectorAlmostEqual(modelPart.Nodes[7].GetValue(KratosMultiphysics.MESH_DISPLACEMENT), [-2.0, 2.0, 6.0])
        self.assertVectorAlmostEqual(modelPart.Nodes[8].GetValue(KratosMultiphysics.MESH_DISPLACEMENT), [-2.0, 2.0, 6.0])

    def test_axis_and_angle(self):
        model_part = GenerateModelPart()

        parameters = KratosMultiphysics.Parameters("""{
            "rotation_definition" : "rotation_axis",
            "rotation_axis"       : [0, 1, 0],
            "reference_point"     : [-1, 0, 0],
            "rotation_angle"      : -1.57079632679,
            "translation_vector"  : [1, 2, 3]
        }""")

        MeshMovingApplication.ImposeMeshMotionProcess(
            model_part,
            parameters
        ).Execute()

    def test_euler_angles(self):
        model_part = GenerateModelPart()

        parameters = KratosMultiphysics.Parameters("""{
            "rotation_definition" : "euler_angles",
            "euler_angles"        : [-1.57079632679, -1.57079632679, 1.57079632679],
            "reference_point"     : [-1, 0, 0],
            "translation_vector"  : [1, 2, 3]
        }""")

        MeshMovingApplication.ImposeMeshMotionProcess(
            model_part,
            parameters
        ).Execute()


if __name__ == "__main__":
    KratosUnittest.main()