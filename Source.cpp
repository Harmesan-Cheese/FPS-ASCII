#include <iostream>
using namespace std;

#include <Windows.h>

int nScreenWidth = 120;			// Console Screen Size X (columns)
int nScreenHeight = 40;			// Console Screen Size Y (rows)

int nMapWidth = 16;				// World Dimensions
int nMapHeight = 16;

float fPlayerX = 8.0f;			// Player Start Position
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;			// Player Start Rotation

float fFOV = 3.14159f / 4.0f;	// Field of View
float fDepth = 16.0f;			// Maximum rendering distance

int main()
{	
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;

	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";






	// Game Loop
	while (1)
	{
		for (int x = 0; x < nScreenWidth; x++)
		{
			// for each column, calculate the projected ray angle into world space
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			float fDistanceToWall = 0.0f;
			bool bHitWall = false;

			float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)
			{

				fDistanceToWall += 0.1f;

				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				// Test if ray is out of bounds
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;			// Just set distance to maximum depth
					fDistanceToWall = fDepth;
				}
				else
				{
					// Ray is inbounds so test to see if the ray cell is wall block
					if (map[nTestY * nMapWidth + nTestX] == '#')
					{
						bHitWall = true;
					}
				}
			}

			// Calculate distance to celling and floor
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y < nCeiling)
					screen[y*nScreenHeight + x] = ' ';
				else if (y > nCeiling && y <= nFloor)
					screen[y*nScreenHeight + x] = '#';
				else
					screen[y*nScreenHeight + x] = ' ';
			}


		}

		// Display Frame
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}