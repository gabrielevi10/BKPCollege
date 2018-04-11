import Data.Char

-- Questao 1
four_eq [x1,x2,x3,x4] = (x1 == x2 && x2 == x3 && x3 == x4) 

-- Questao 2

-- Questao 3
resto :: Integer -> Integer -> Integer
resto x y = z where
	k = quot x y
	z = x-k*y

-- Questao 4
twopotn n 
 	| n == 0 = 1
	| otherwise = 2 * twopotn (n-1)

-- Questao 5
double_list_sum xs = (foldl1(+) xs)*2

-- Questao 6
is_leapyear = do 
				putStr "Enter the year for test: "
				year <- readLn :: IO Int
				if mod year 4 == 0 && mod year 100 /= 0 || mod year 400 == 0 then
					putStr "Yes, its a leap year\n"
				else
					putStr "Its not a leap year\n"

-- Questao 7
divisors x = xs where xs = [k | k <- [1..x], mod x k == 0]

-- Questao 8
