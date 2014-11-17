
#include <math.h>
#include <math.h>

#include <scttypes.h>

SDL_Integer getDistance(SDL_Integer xps,SDL_Integer yps,
						SDL_Integer xpf,SDL_Integer ypf)
{
	return (SDL_Integer)floor(sqrt(pow((xpf - xps),2) + pow((ypf - yps),2)));
}

SDL_Integer sdl_sqrt(SDL_Integer x)
{
	return (SDL_Integer)floor(sqrt(x));
}

SDL_Integer getHeading(void)
{
	return (SDL_Integer)(rand()%8);
}
