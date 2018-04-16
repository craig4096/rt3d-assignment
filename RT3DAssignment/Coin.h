#pragma once
#include "Vector3.h"
#include <fstream>
#include "Material.h"

class Coin
{
public:
	Coin(std::ifstream& ifs);
	Coin(void);
	~Coin(void);


	void Update(float dTime);
	void Draw();

	void Collect();
	bool IsCollected() const;

	void SetPosition(const Vector3& pos);

	const Vector3& GetPosition() const { return position; }

private:

	bool collected;
	float rotation;
	float rotationSpeed;
	float radius;
	float thickness;

	Material material;

	Vector3 position;
};
