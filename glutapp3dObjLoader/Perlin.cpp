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

	void sum(int k)
	{
		double temp[3] = {0.0, 0.0, 0.0};
		for (int i  = 0; i < k; ++i)
		{
			resample();
			temp[0] += x;
			temp[1] += y;
			temp[2] += z;
		}
		x = temp[0];
		y = temp[1];
		z = temp[2];
	}

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
	Grid3(int n = 10)
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

	void shift(bool west, bool east, bool north, bool south)
	{
		if (west)
		{

			for (int j = 0; j < N+1; ++j)
			{
				for (int k = 0; k < N+1; ++k)
				{
					cube[N][j][k] = cube[0][j][k];
 				}
			}
			for (int i = 0; i < N; ++i)
			{
				for (int j = 0; j < N+1; ++j)
				{
					for (int k = 0; k < N+1; ++k)
					{
						cube[i][j][k].resample(); 
					}
				}
			}
		}
		if (east)
		{
			for (int j = 0; j < N+1; ++j)
			{
				for (int k = 0; k < N+1; ++k)
				{
					cube[0][j][k] = cube[N][j][k];
 				}
			}
			for (int i = 1; i < N+1; ++i)
			{
				for (int j = 0; j < N+1; ++j)
				{
					for (int k = 0; k < N+1; ++k)
					{
						cube[i][j][k].resample(); 
					}
				}
			}
		}
		if (north)
		{
			for (int i = 0; i < N+1; ++i)
			{
				for (int k = 0; k < N+1; ++k)
				{
					cube[i][0][k] = cube[i][N][k];
 				}
			}
			for (int i = 0; i < N+1; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					for (int k = 0; k < N+1; ++k)
					{
						cube[i][j][k].resample(); 
					}
				}
			}
		}
		if (south)
		{
			for (int i = 0; i < N+1; ++i)
			{
				for (int k = 0; k < N+1; ++k)
				{
					cube[i][N][k] = cube[i][0][k];
 				}
			}
			for (int i = 0; i < N+1; ++i)
			{
				for (int j = 1; j < N+1; ++j)
				{
					for (int k = 0; k < N+1; ++k)
					{
						cube[i][j][k].resample(); 
					}
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

class Terrain_plain
{
	Grid3 cube;
	int heightmap[25][25];
public:
	int dirt_id;
	int stone_id;
	int air_id;
	int dim[3];
	int chunk[25][25][10];
	Terrain_plain(int seed_size = 10)
	{
		dim[0] = 25; dim[1] = 25; dim[2] = 10;
		air_id = 0;
		cube = Grid3(seed_size);
		cube.initialize();
		double min, max;
		min = max = cube.perlin3(0, 0, 0);
		for (int i = 0; i < dim[0]; ++i)
		{
			for (int j = 0; j < dim[1]; ++j)
			{
				for (int k = 0; k < dim[2]; ++k)
				{
					double val = cube.perlin3(i/(double)dim[0], j/(double)dim[1], k/(double)dim[2]);
					if (val > max)
						max = val;
					if (val < min)
						min = val;
				}
			}
		}

		double surface_mean = 0;
		for (int i = 0; i < dim[0]; ++i)
		{
			for (int j = 0; j < dim[1]; ++j)
			{
				double height = 0;
				for (int k = 0; k < dim[2]; ++k)
				{
					double val = (cube.perlin3(i/(double)dim[0], j/(double)dim[1], k/(double)dim[2])-min)/(max-min);
					height += val;
					chunk[i][j][k] = (val < 0.5)? 1 : 2;
				}
				heightmap[i][j] = dim[2] - height/2;
				surface_mean += chunk[i][j][0];
			}
		}

		surface_mean /= 625;
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

		for (int i = 0; i < dim[0]; ++i)
		{
			for (int j = 0; j < dim[1]; ++j)
			{
				press(i, j, dim[2]-heightmap[i][j]);
			}
		}
	}

	// generates 24 blocks in a direction
	// +i: north
	// +j: east
	void load(bool west = false, bool east = false, bool north = false, bool south = false)
	{
		cube.shift(west, east, north, south);
		double min, max;
		min = max = cube.perlin3(0, 0, 0);
		for (int i = 0; i < dim[0]; ++i)
		{
			for (int j = 0; j < dim[1]; ++j)
			{
				for (int k = 0; k < dim[2]; ++k)
				{
					double val = cube.perlin3(i/(double)dim[0], j/(double)dim[1], k/(double)dim[2]);
					if (val > max)
						max = val;
					if (val < min)
						min = val;
				}
			}
		}

		double surface_mean = 0;
		for (int i = 0; i < dim[0]; ++i)
		{
			for (int j = 0; j < dim[1]; ++j)
			{
				double height = 0;
				for (int k = 0; k < dim[2]; ++k)
				{
					double val = (cube.perlin3(i/(double)dim[0], j/(double)dim[1], k/(double)dim[2])-min)/(max-min);
					height += val;
					chunk[i][j][k] = (val < 0.5)? 1 : 2;
				}
				heightmap[i][j] = dim[2] - height/2;
				surface_mean += chunk[i][j][0];
			}
		}

		surface_mean /= 625;
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

		for (int i = 0; i < dim[0]; ++i)
		{
			for (int j = 0; j < dim[1]; ++j)
			{
				press(i, j, dim[2]-heightmap[i][j]);
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
			if ( d == 0)
				chunk[i][j][0] = dirt_id;
			else
				chunk[i][j][0] = air_id;
		}
	}

	double set(int i, int j, int k, int type = 0, int width = 5)
	{
		int radius = width/2 + 1;
		int mean, count;
		mean = count = 0;

		for (int x = i-radius; x < i+radius; ++x)
		{
			for (int y = j-radius; y < j + radius; ++y)
			{
				for (int z = k-radius; z < k +radius; ++z)
				{
					++count;
					// cout << sqrt((x-i)*(x-i) + (y-j)*(y-j) + (z-k)*(z-k));
					// cout << " at ( " << x << ", " << y << ", " << z << ")... ";
					if ((x > 0 && x < dim[0]) && (y > 0 && y < dim[1]) && (z > 0 && z < dim[2]))
					{
						if (sqrt((x-i)*(x-i) + (y-j)*(y-j) + (z-k)*(z-k)) <= radius)
						{
							// cout << "hit" << endl;
							chunk[x][y][z] = type;
						}
						// else
							// cout << "miss" << endl;
						++mean;
					}
					else
					{
						// cout << "miss" << endl;
					}
				}
			}
		}
		mean /= count;

		return mean;
	}

	int get(int i, int j, int k)
	{
		return chunk[i][j][k];
	}

	void print()
	{
		cout << "Terrain: " << endl;
		for (int k = 0; k < dim[2]; ++k)
		{
			for (int j = dim[1]*0.25; j < dim[1]*0.75; ++j)
			{
				for (int i = dim[0]*0.25; i < dim[0]*0.75; ++i)
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


// either momentum or perlin
class Worm
{
	//Grid3 cube;
	GradVec vec, mis;
	int lifespan;
	int step;
	int width;
	double momentum, jitter;
	int pos[3];

public:
	Worm()
	{
		//cube = Grid3(seed_size);
		step = 5;
		lifespan = 170;
		width = 5;
		momentum = 0.5;
		jitter = 0.09;
	}

	void crawl(Terrain_plain& data ,int x = 0, int y = 0, int z = 0)
	{
		pos[0] = x; pos[1] = y; pos[2] = z;
		double old[3] = {0.0, 0.0, 0.0};

		data.set(pos[0], pos[1], pos[2]);

		while (lifespan > 0)
		{
			// cout << "lifespan: " << lifespan << endl;
			vec.sum(width);
			//cout << "direction: ";
			// vec.print();
			// cout << endl;
			for (int i = 0; i < step; ++i)
			{
				mis.sum(width);
				pos[0] += (int)(vec.x + old[0]*0.17*momentum + mis.x*jitter);
				pos[1] += (int)(vec.y + old[1]*0.17*momentum + mis.y*jitter);
				pos[2] += (int)(vec.z + old[2]*0.17*momentum + mis.z*jitter);

				momentum += momentum * (data.set(pos[0], pos[1], pos[2])-0.5);

				lifespan--;
			}
			old[0] = vec.x;
			old[1] = vec.y;
			old[2] = vec.z;
		}

		lifespan = 170;
	}
};

class Terrain
{
	Terrain_plain data;
	vector<Worm> diggers;

	mt19937 generator;

public:
	int dirt_id;
	int stone_id;
	int air_id;
	Terrain(int worms = 17)
	{
		random_device rd;
		generator.seed(rd()/17);

		data = Terrain_plain();
		for (int i = 0; i < worms; ++i)
		{
			diggers.push_back(Worm());
			diggers[i].crawl(data, (int)generator()%data.dim[0], (int)generator()%data.dim[1], (int)generator()%data.dim[2]);
		}

		// digger.crawl(data);
		//diggers.crawl(data, data.dim[0]/2, data.dim[1]/2, data.dim[2]-1);

		dirt_id = data.dirt_id;
		stone_id = data.stone_id;
		air_id = data.air_id;
	}

	void load(bool west, bool east, bool north, bool south)
	{
		data.load(west, east, north, south);

		for (int i = 0; i < diggers.size(); ++i)
		{
			// cout << "digger #" << i << endl;
			// diggers.push_back(Worm());
			diggers[i].crawl(data, (int)generator()%data.dim[0], (int)generator()%data.dim[1], (int)generator()%data.dim[2]);
		}
	}

	int get(int i, int j, int k){return data.get(i,j,k);}
	void print(){data.print();}
};

// int main(int argc, char const *argv[])
// {
// 	Terrain data = Terrain(7);

// 	data.print();

// 	data.load(true, false, false, false);

// 	data.print();

// 	return 0;
// }