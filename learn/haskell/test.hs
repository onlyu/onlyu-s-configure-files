import Data.List

r = 5.0
area = pi * r ^ 2
       
heron a b c = sqrt (s*(s-a)*(s-b)*(s-c))
  where
    s = (a+b+c) / 2

areaTriangleTrig  a b c = c * height / 2   -- use trigonometry
  where
    cosa   = (b^2 + c^2 - a^2) / (2*b*c)
    sina   = sqrt (1 - cosa^2)
    height = b*sina

areaTriangleHeron a b c = result           -- use Heron's formula
  where
    result = sqrt (s*(s-a)*(s-b)*(s-c))
    s      = (a+b+c)/2
    
{-
abs x
    | x < 0     = 0 - x
    | otherwise = x
-}

numOfSolutions a b c
    | disc > 0  = 2
    | disc == 0 = 1
    | otherwise = 0
  where
    disc = b^2 - 4*a*c

mySigNum x
  | x > 0 = 1
  | x < 0 = -1
  | otherwise = 0

myABS x
  | x < 0 = -x
  | otherwise = x

main = do
  putStrLn "What's your name?"
  name <- getLine
  putStrLn ("Hello " ++ name ++ ", how are you?")

foo = do
  putStrLn "The base?"
  base <- getLine
  putStrLn "The height?"
  height <- getLine
  putStrLn ("The area of that triangle is " ++ (show ((read base) * (read height) / 2)))

greeting = do
  putStrLn "What's your name?"
  name <- getLine
  doGreeting name

doGreeting "Simon" = putStrLn "Haskell is a great programming language."
doGreeting "John" =  putStrLn "Haskell is a great programming language."
doGreeting "Phil" =  putStrLn "Haskell is a great programming language."
doGreeting "Koen" =  putStrLn "debugging Haskell is fun."
doGreeting _ = putStrLn "I don't known who you are"

test :: [[Int]]
test = nub $ map splitToNum $ perm [1..9] 

perm :: Eq a => [a] -> [[a]]
perm [] = [[]]
perm xs = concat [map (x:) $ perm $ delete x xs | x <- xs]

splitToNum :: [Int] -> [Int]
splitToNum xs = sort $ map toNum [head3, middle3, tail3]
    where head3 = take 3 xs
          middle3 = take 3 $ drop 3 xs
          tail3 = drop 6 xs
          toNum (x:y:z:[]) = x * 100 + y * 10 + z 
