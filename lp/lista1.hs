import Data.Char

-- Questao 1
four_eq [x1,x2,x3,x4] = (x1 == x2 && x2 == x3 && x3 == x4) 

-- Questao 2
[] /\ _ = []
_ /\ [] = []
(x:xs) /\ ys
	| elem x ys = [x] ++ xs/\ys
	| otherwise = xs/\ys

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
same_elements [] ys = True
same_elements xs [] = False
same_elements (x:xs)(ys)  
	| elem x ys = same_elements xs ys
	| otherwise =  False

-- Questao 9
formatof :: String -> String
formatof (x:xs)
	| x == '.' = xs
	| otherwise = formatof xs

-- Questao 10
timesin x [] = 0
timesin x (y:ys)
	| x == y = 1 + timesin x ys
	| otherwise = timesin x ys

-- Questao 11
above_average xs = [k | k <- xs, k > av] where av = div (foldl1(+) xs)(length xs)

-- Questao 12
namesin [] = []
namesin (x:xs)
	| elem x ['A'..'Z'] = [x]++getString xs
	| otherwise = namesin xs

getString (y:ys) 
	| y == ' ' = []
	| otherwise = [y]++getString ys

-- Questao 13
position x (y:ys) 
	| x /= y = 1+position x ys
	| x == y = 0

-- Questao 14
oddeven xs = (filter even xs, filter odd xs)

-- Questão 22-26
wherer = k where k = 1
letter = let bissection a b = (a+b)/2 in bissection 2 4 
mapper a = map(\x -> x^2)a
composer a = sum.map(\x -> x^2)a