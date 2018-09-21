#include	"gz.h"
#define one 0
#define two 1
#define three 2

class DDA
{
public:
	DDA(GzCoord start, GzCoord end);
	void setCurrent(GzCoord start);
	GzCoord* getCurrent();
	virtual ~DDA();

private:
	GzCoord start;
	GzCoord end;
	GzCoord current;
	float slopeZ;
	bool isLeftEdge;

	void setStart(GzCoord start);
	void setEnd(GzCoord end);
	void setSlopeX();
	void setSlopeZ();

	inline float slope(GzCoord vertex1, GzCoord vertex2, int rise, int run) {
		return (vertex2[run] == vertex1[run]) // divide by zero guard
			? NULL
			: (vertex2[rise] - vertex1[rise]) / (vertex2[run] - vertex1[run]);
	}
};

