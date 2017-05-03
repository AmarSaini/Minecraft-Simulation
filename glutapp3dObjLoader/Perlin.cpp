# include <math.h>
# include <random>
# include <vector>

# include <iostream>

using namespace std;




class GradVec
{
	mt19937 generator;

	double twister(double d)
	{
		return generator()/d;
	}
public:
	double x, y, z;
	GradVec()
	{
		random_device rd;
		generator.seed(rd());
		x = twister(1000.0);
		y = twister(1000.0);
		z = twister(1000.0);

		double mag = sqrt(x*x + y*y + z*z);
		x /= mag;
		y /= mag;
		z /= mag;
	}

	void resample()
	{
		x = twister(1000.0);
		y = twister(1000.0);
		z = twister(1000.0);

		double mag = sqrt(x*x + y*y + z*z);
		x /= mag;
		y /= mag;
		z /= mag;
	}

	void random_samp()
	{
		x = twister(1000.0);
		y = twister(1000.0);
		z = twister(1000.0);

		double mag = sqrt(x*x + y*y + z*z);
		x /= mag;
		y /= mag;
		z /= mag;	}

	void print()
	{
		cout << "(" << x << ", " << y << ", "<< z << ")";
	}
};

class Grid3
{
	int N; // dimension of the 3d cube grid;
	vector< vector< vector<GradVec> > > cube;
public:
	Grid3(int n = 100)
	{
		N = n;
		vector< vector< vector<GradVec> > > tempVec(N+1, vector< vector<GradVec> >(N+1, vector<GradVec>(N+1)));
		cube = tempVec;
	}

	void initialize(bool twister = true)
	{
		if (twister)
			for (int i = 0; i < N+1; ++i)
			{
				for (int j = 0; j < N+1; ++j)
				{
					for (int k = 0; k < N+1; ++k)
					{
						cube[i][j][k] = GradVec();
					}
				}
			}
		else
			for (int i = 0; i < N+1; ++i)
			{
				for (int j = 0; j < N+1; ++j)
				{
					for (int k = 0; k < N+1; ++k)
					{
						cube[i][j][k] = GradVec();
						cube[i][j][k].random_samp();
					}
				}
			}
	}

	void print()
	{
		for (int k = 0; k < N+1; ++k)
		{
			for (int j = 0; j < N+1; ++j)
			{
				for (int i = 0; i < N+1; ++i)
				{
					cube[i][j][k].print();
					cout << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}

	double fade(double t)
	{
		// return t;
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	// input:
	// x: scalar in [0, 1]
	// y: scalar in [0, 1]
	// z: scalar in [0, 1]
	// output:
	// avg: scalar in [0, 1] 
	double perlin3(double x, double y, double z)
	{
		int index[3] = {floor(x*(N-1) + 0.5), floor(y*(N-1) + 0.5), floor(z*(N-1) + 0.5)};
		// cout << "index (" << index[0] << ", " << index[1] << ", " << index[2] << ")" << endl;
		double pos[3] = {x-(index[0]/N), y-(index[1]/N), z-(index[2]/N)};
		// cout << "pos (" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")" << endl;

		/*
			000	100		001	101

			010	110		011	111
		*/

		double dx, dy, dz;
		GradVec gv;

		// 000 topleft
		dx = 0-pos[0];
		dy = 1-pos[1];
		dz = 0-pos[2];
		gv = cube[index[0]][index[1]][index[2]];

		double dot000 = gv.x * dx + gv.y * dy + gv.z * dz;

		// 100 topright
		dx = 1-pos[0];
		dy = 1-pos[1];
		dz = 0-pos[2];
		gv = cube[index[0]+1][index[1]][index[2]];

		double dot100 = gv.x * dx + gv.y * dy + gv.z * dz;

		// 010 botleft
		dx = 0-pos[0];
		dy = 0-pos[1];
		dz = 0-pos[2];
		gv = cube[index[0]][index[1]+1][index[2]];

		double dot010 = gv.x * dx + gv.y * dy + gv.z * dz;

		// 110 botright
		dx = 1-pos[0];
		dy = 0-pos[1];
		dz = 0-pos[2];
		gv = cube[index[0]][index[1]+1][index[2]];

		double dot110 = gv.x * dx + gv.y * dy + gv.z * dz;

		// 001 topleftback
		dx = 0-pos[0];
		dy = 1-pos[1];
		dz = 1-pos[2];
		gv = cube[index[0]][index[1]][index[2]+1];

		double dot001 = gv.x * dx + gv.y * dy + gv.z * dz;

		// 101 toprightback
		dx = 1-pos[0];
		dy = 1-pos[1];
		dz = 1-pos[2];
		gv = cube[index[0]+1][index[1]][index[2]+1];

		double dot101 = gv.x * dx + gv.y * dy + gv.z * dz;

		// 011 botleftback
		dx = 0-pos[0];
		dy = 0-pos[1];
		dz = 1-pos[2];
		gv = cube[index[0]][index[1]+1][index[2]+1];

		double dot011 = gv.x * dx + gv.y * dy + gv.z * dz;

		// 111 botrightback
		dx = 1-pos[0];
		dy = 0-pos[1];
		dz = 1-pos[2];
		gv = cube[index[0]+1][index[1]+1][index[2]+1];

		double dot111 = gv.x * dx + gv.y * dy + gv.z * dz;

		// interpolate dots
		double u = fade(x);
		double v = fade(y);
		double w = fade(z);

		double topfront = dot000 + u*(dot100-dot000);
		double botfront = dot010 + u*(dot110-dot010);
		double front = topfront + v*(botfront-topfront);

		double topback = dot001 + u*(dot101-dot001);
		double botback = dot011 + u*(dot111-dot011);
		double back = topback + v*(botback-topback);

		double avg = front + w*(back-front);

		return avg;
	}
};

class Chunk
{
	int chunk[25][25][10];
	int categories;

	Grid3 grid;

public:
	Chunk(int categories = 2)
	{
		grid.initialize(false);
		this->categories = categories;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				for (int k = 0; k < 10; ++k)
				{
					double val = grid.perlin3(i/25.0, j/25.0, k/10.0);
					for (int c = 1; c <= categories; ++c)
					{
						// cout << i << ", " << j << ", " << k << endl;
						if (val < c*1.0/categories)
						{
							chunk[i][j][k] = c;
							break;
						}
					}
				}
			}
		}
	}
	void normalize()
	{
		double max, min;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				for (int k = 0; k < 10; ++k)
				{
					double val = grid.perlin3(i/25.0, j/25.0, k/10.0);
					if (i == 0 && j == 0 && k == 0)
						max = min = val;
					else
					{
						if (val > max)
						{
							max = val;
						}
						if (val < min)
						{
							min = val;
						}
					}
				}
			}
		}

		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				for (int k = 0; k < 10; ++k)
				{
					double val = grid.perlin3(i/25.0, j/25.0, k/10.0);
					val = (val-min)/(max-min); 
					for (int c = 1; c <= categories; ++c)
					{
						if (val < c*1.0/categories)
						{
							chunk[i][j][k] = c;
							break;
						}
					}
				}
			}
		}
	}

	int get(int i, int j, int k)
	{
		return chunk[i][j][k];
	}
};

class Terrain
{
	int chunk[50][50][10];
	Grid3 cube;
	int heightmap[50][50];
public:
	int dirt_id;
	int stone_id;
	int air_id;
	Terrain()
	{
		air_id = 0;
		cube.initialize();
		double min, max;
		min = max = cube.perlin3(0, 0, 0);
		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				for (int k = 0; k < 10; ++k)
				{
					double val = cube.perlin3(i/50.0, j/50.0, k/10.0);
					if (val > max)
						max = val;
					if (val < min)
						min = val;
				}
			}
		}

		double surface_mean = 0;
		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				double height = 0;
				for (int k = 0; k < 10; ++k)
				{
					double val = (cube.perlin3(i/50.0, j/50.0, k/10.0)-min)/(max-min);
					height += val;
					chunk[i][j][k] = (val < 0.5)? 1 : 2;
				}
				heightmap[i][j] = 10 - height/2;
				surface_mean += chunk[i][j][0];
			}
		}

		surface_mean /= 2500;
		if (surface_mean < 1.5)
		{
			dirt_id = 1;
			stone_id = 2;
		}
		else
		{
			dirt_id = 2;
			stone_id = 1;
		}

		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				press(i, j, 10-heightmap[i][j]);
			}
		}
	}

	void press(int i, int j, int down)
	{
		if (down < 0) return;
		for (int d = 0; d < down; ++d)
		{
			for (int k = 9; k > 0; k--)
			{
				chunk[i][j][k] = chunk[i][j][k-1];
			}
			chunk[i][j][0] = 0;
		}
	}

	int get(int i, int j, int k)
	{
		return chunk[i][j][k];
	}

	void print()
	{
		cout << "Terrain: " << endl;
		for (int k = 0; k < 10; ++k)
		{
			for (int j = 0; j < 20; ++j)
			{
				for (int i = 0; i < 20; ++i)
				{
					if (chunk[i][j][k] == air_id)
						cout << "  air  ,";
					else if (chunk[i][j][k] == stone_id)
						cout << " stone ,";
					else if (chunk[i][j][k] == dirt_id)
						cout << "  dirt ,";
				}
				cout << endl;
			}
			cout << "-----------------" << endl;
		}
	}
};