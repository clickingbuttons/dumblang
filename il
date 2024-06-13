fn main {
	ret1: u8 = 0

	add ret1, 1
	add ret1, 2
	add ret1, 3
	add ret1, 4
	add ret1, 5
	add ret1, 6
	add ret1, 7
	add ret1, 8
	add ret1, 9
	add ret1, 10
	to_u8 tmp, 11.2, .floor
	add ret1, tmp
	to_u8 tmp, 12.3, .ceil
	add ret1, tmp
	add ret1, 13
	add ret1, 1
}
