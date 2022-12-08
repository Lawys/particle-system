typedef float4 point;
typedef float4 vector;

float3 randVector(float min, float max)
{
	max *= 100, min *= 100;
	int gid = get_global_id(0), seed = 4 + gid;
	int t = seed ^ (seed << 11), f = 2 ^ (2 >> 19) ^ (t ^ (t >> 8));
	float x = ((f % (int)(max - min)) + min) / 100.0f;
	t = seed ^ (seed << 13), f = 2 ^ (2 >> 17) ^ (t ^ (t >> 4));
	float y = ((f % (int)(max - min)) + min) / 100.0f;
	t = seed ^ (seed << 14), f = 2 ^ (2 >> 16) ^ (t ^ (t >> 2));
	float z = ((f % (int)(max - min)) + min) / 100.0f;
	return (float3) { x, y, z };
}

__kernel
void start(__global point* devicePointsObject, const float Gx, const float Gy, const float Gz)
{
	int gid = get_global_id(0);
	devicePointsObject[gid] = (point){ randVector(-100, 100) / 100.0f , 1.0f } + (point){ Gx, Gy, Gz, 0.0f };
}

__kernel
void particlesToCube(
	__global point* devicePointsObject,
	__global vector* deviceVectorsObject,
	const float Gx, const float Gy, const float Gz, const float Gp
	)
{
	// get fixed rand final point
	int gid = get_global_id(0);
	point p = { randVector(-100, 100), 1.0f }, p0 = devicePointsObject[gid];
	float d = fabs(p.x);
	if (fabs(p.y) > d) d = fabs(p.y);
	if (fabs(p.z) > d) d = fabs(p.z);
	if (d != 0.0) d /= 100.0f;
	p = (point){ p.x /= d, p.y /= d, p.z /= d, 1.0f } + (point){ Gx, Gy, Gz, 0.0f }; // final point
	// calculate vertor attraction and apply
	vector v = { (p.x - p0.x), (p.y - p0.y), (p.z - p0.z), 0.0 }; // get vector p0 to p
	d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); // get distance between p0 and p
	deviceVectorsObject[gid] += (v / d) * Gp; // add
	vector v0 = deviceVectorsObject[gid];
	float dd = sqrt(v0.x * v0.x + v0.y * v0.y + v0.z * v0.z);
	// if total vector > distance, point = final point	
	if (dd < d)
		devicePointsObject[gid] += deviceVectorsObject[gid] + (v / 10.0f) * Gp;
	else
		devicePointsObject[gid] = p,
		deviceVectorsObject[gid] = (vector){ 0.0f, 0.0f ,0.0f ,0.0f };
};

__kernel
void particlesToCubeFilled(
	__global point* devicePointsObject,
	__global vector* deviceVectorsObject,
	const float Gx, const float Gy, const float Gz, const float Gp
	)
{
	// get fixed rand final point
	int gid = get_global_id(0);
	point p0 = devicePointsObject[gid];
	point p = (point){ randVector(-100, 100), 1.0f } + (point){ Gx, Gy, Gz, 0.0f }; // final point
	// calculate vertor attraction and apply
	vector v = { (p.x - p0.x), (p.y - p0.y), (p.z - p0.z), 0.0 }; // get vector p0 to p
	float d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); // get distance between p0 and p
	deviceVectorsObject[gid] += (v / d) * Gp; // add
	vector v0 = deviceVectorsObject[gid];
	float dd = sqrt(v0.x * v0.x + v0.y * v0.y + v0.z * v0.z);
	// if total vector > distance, point = final point	
	if (dd < d)
		devicePointsObject[gid] += deviceVectorsObject[gid] + (v / 10.0f) * Gp;
	else
		devicePointsObject[gid] = p,
		deviceVectorsObject[gid] = (vector){ 0.0f, 0.0f ,0.0f ,0.0f };
};

__kernel
void particlesToSphere(
	__global point* devicePointsObject,
	__global vector* deviceVectorsObject,
	const float Gx, const float Gy, const float Gz, const float Gp
	)
{
	// get fixed rand final point
	int gid = get_global_id(0);
	point p = { randVector(-120.71f, 120.71f), 1.0f }, p0 = devicePointsObject[gid];
	float d = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	p = (point){ (p.x / d) * 120.71f, (p.y / d) * 120.71f, (p.z / d) * 120.71f, 1.0f } + (point){ Gx, Gy, Gz, 0.0f }; // final point
	// calculate vertor attraction and apply
	vector v = { (p.x - p0.x), (p.y - p0.y), (p.z - p0.z), 0.0 }; // get vector p0 to p
	d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); // get distance between p0 and p
	deviceVectorsObject[gid] += (v / d) * Gp; // add
	vector v0 = deviceVectorsObject[gid];
	float dd = sqrt(v0.x * v0.x + v0.y * v0.y + v0.z * v0.z);
	// if total vector > distance, point = final point	
	if (dd < d)
		devicePointsObject[gid] += deviceVectorsObject[gid] + (v / 10.0f) * Gp;
	else
		devicePointsObject[gid] = p,
		deviceVectorsObject[gid] = (vector){ 0.0f, 0.0f ,0.0f ,0.0f };
};

__kernel
void particlesToSphereFilled(
	__global point* devicePointsObject,
	__global vector* deviceVectorsObject,
	const float Gx, const float Gy, const float Gz, const float Gp
	)
{
	// get fixed rand final point
	int gid = get_global_id(0);
	point p0 = devicePointsObject[gid];
	point p = (point){ randVector(-120.71f, 120.71f), 1.0f }; // final point
	float d = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	if (d > 120.71f) d /= 120.71f, p = (point){ p.x /= d, p.y /= d, p.z /= d, 1.0f };
	p += (point){ Gx, Gy, Gz, 0.0f };
	// calculate vertor attraction and apply
	vector v = { (p.x - p0.x), (p.y - p0.y), (p.z - p0.z), 0.0 }; // get vector p0 to p
	d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); // get distance between p0 and p
	deviceVectorsObject[gid] += (v / d) * Gp; // add
	vector v0 = deviceVectorsObject[gid];
	float dd = sqrt(v0.x * v0.x + v0.y * v0.y + v0.z * v0.z);
	// if total vector > distance, point = final point	
	if (dd < d)
		devicePointsObject[gid] += deviceVectorsObject[gid] + (v / 10.0f) * Gp;
	else
		devicePointsObject[gid] = p,
		deviceVectorsObject[gid] = (vector){ 0.0f, 0.0f ,0.0f ,0.0f };
};

__kernel
void particlesCleanTimers(
	__global float* deviceTimersObject
	)
{
	int gid = get_global_id(0);
	deviceTimersObject[gid] = 0.0f;
}

__kernel
void particlesFountain(
	__global point* devicePointsObject,
	__global vector* deviceVectorsObject,
	__global float* deviceTimersObject,
	const float Gx, const float Gy, const float Gz, const float Gp
	)
{
	int gid = get_global_id(0);
	if (deviceTimersObject[gid] <= 0.0f) {
		deviceTimersObject[gid] = randVector(0, 100.0f).y;
		point p = (point){ randVector(-120.71f, 120.71f), 1.0f }; // final point
		float d = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
		if (d > 120.71f) d /= 120.71f, p = (point){ p.x /= d, p.y /= d, p.z /= d, 1.0f }; p += (point){ Gx, Gy, Gz, 0.0f };
		deviceVectorsObject[gid] = (vector) { (p.x - Gx), (p.y - Gy), (p.z - Gz), 0.0f } / 10.0f;
		devicePointsObject[gid] = (point){ 0.0f, 0.0f, 0.0f, 1.0f } + (point){ Gx, Gy, Gz, 0.0f };
	}
	deviceTimersObject[gid] -= Gp;
	//deviceVectorsObject[gid] += (vector) { 0.0, -0.98, 0.0, 0.0 };
	devicePointsObject[gid] += deviceVectorsObject[gid] * Gp;
};
	
__kernel
void particles(
	__global point * devicePointsObject,
	__global vector * deviceVectorsObject,
	const float Gx, const float Gy, const float Gz, const float Gp
	)
{
	int gid = get_global_id(0); // particle #
	point p0 = devicePointsObject[gid];
	vector v = { (Gx - p0.x), (Gy - p0.y), (Gz - p0.z), 0.0 };
	float d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); v /= d;
	deviceVectorsObject[gid] += v * Gp;
	//vector v0 = deviceVectorsObject[gid];
	//float dd = sqrt(v0.x * v0.x + v0.y * v0.y + v0.z * v0.z);
	//if (dd <= d)
	//	devicePointsObject[gid] += deviceVectorsObject[gid];
	//else
	//	deviceVectorsObject[gid] -= v * Gp * (1.0f - d / dd),
		devicePointsObject[gid] += deviceVectorsObject[gid];
};