#include "Softbody.h"
#include "Sphere.h"
#include "Spring.h"

void Softbody::Build(PhysicsScene* scene, glm::vec2 position, float spacing, float springForce, float damping, std::vector<std::string>& strings)
{
	size_t numOfColumns = strings.size();
	size_t numOfRows = strings[0].length();
									//We have to static cast to get rid of a warning
	Sphere** spheres = new Sphere* [static_cast<unsigned __int64>(numOfRows) * static_cast<unsigned __int64>(numOfColumns)];
	//Create the spheres
	for(int row = 0; row < numOfRows; row++)
		for (int column = 0; column < numOfColumns; column++)
		{
			switch (strings[column][row])
			{
			case '0':
			{
				spheres[row * numOfColumns + column] = new Sphere(position + glm::vec2(row, column) * spacing, false, glm::vec2(0, 0), 1, (spacing - 1) / 2.0f, glm::vec4(1, 0, 0, 1));
				scene->AddActor(spheres[row * numOfColumns + column]);
				break;
			}
			default:
			{
				spheres[row * numOfColumns + column] = nullptr;
				break;
			}
			}
		}

	//Create the springs
	for (int row = 1; row < numOfRows; row++)
		for (int column = 1; column < numOfColumns; column++)
		{
			Sphere* s11 = spheres[row * (int)numOfColumns + column];
			Sphere* s01 = spheres[(row - 1) * (int)numOfColumns + column];
			Sphere* s10 = spheres[row * (int)numOfColumns + column - 1];
			Sphere* s00 = spheres[(row - 1) * (int)numOfColumns + column - 1];

			bool endOjJ = column == numOfColumns - 1;
			bool endOfI = row == numOfRows - 1;

			Sphere* s22 = (!endOfI && !endOjJ) ? spheres[(row + 1) * (int)numOfColumns + (column + 1)] : nullptr;
			Sphere* s02 = !endOjJ ? spheres[(row - 1) * (int)numOfColumns + (column + 1)] : nullptr;
			Sphere* s20 = !endOfI ? spheres[(row + 1) * (int)numOfColumns + (column - 1)] : nullptr;

			if (s11 && s01)
				scene->AddActor(new Spring(s11, s01, damping, springForce));
			if (s11 && s10)
				scene->AddActor(new Spring(s11, s10, damping, springForce));
			if (s10 && s00)
				scene->AddActor(new Spring(s10, s00, damping, springForce));
			if (s01 && s00)
				scene->AddActor(new Spring(s01, s00, damping, springForce));

			if (s22 && s02)
				scene->AddActor(new Spring(s22, s02, damping, springForce));
			if (s22 && s20)
				scene->AddActor(new Spring(s22, s20, damping, springForce));
			if (s20 && s00)
				scene->AddActor(new Spring(s20, s00, damping, springForce));
			if (s02 && s00)
				scene->AddActor(new Spring(s02, s00, damping, springForce));

			if (s00 && s11)
				scene->AddActor(new Spring(s00, s11, damping, springForce));
			if (s01 && s10)
				scene->AddActor(new Spring(s01, s10, damping, springForce));
		}
}
