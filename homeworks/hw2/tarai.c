unsigned long int tarai( unsigned long int x,
                         unsigned long int y,
                         unsigned long int z ) {
	if (y < x)
	return tarai( tarai( x-1, y, z ),
                  tarai( y-1, z, x ),
                  tarai( z-1, x, y ));
	else return y;
}
