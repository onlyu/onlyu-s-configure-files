

import Data.List

-- Find the last element of a list.
myLast :: [a] -> a
myLast [x] = x
myLast (x:xs) = myLast xs

-- Find the last but one element of a list.
myButLast :: [a] -> a
myButLast = last . init

-- Find the K'th element of a list. The first element in the list is number 1.
elementAt :: [a] -> Int -> a
elementAt list n = list !! (n-1)

-- Find the number of elements of a list.
myLength :: [a] -> Int
myLength [] = 0
myLength (x:xs) = 1 + myLength xs

-- Reverse a list.
myReverse :: [a] -> [a]
myReverse = foldl (flip (:)) []

-- Find out whether a list is a palindrome. A palindrome can be read forward or backward; e.g. (x a m a x).
isPalindrome :: Eq a => [a] -> Bool
isPalindrome x = x == (reverse x)

-- Flatten a nested list structure.
data NestedList a = Elem a | List [NestedList a]
flatten :: NestedList a -> [a]
flatten (Elem x) = [x]
flatten (List x) = concatMap flatten x

-- Eliminate consecutive duplicates of list elements.
compress :: Eq a => [a] -> [a]
compress = map head . group

pack :: Eq a => [a] -> [[a]]
pack (x:xs) = let (first, rest) = span (==x) xs in (x:first) : pack rest
pack [] = []

encode :: Eq a => [a] -> [(Int,a)]
encode = map (\x -> (length x, head x)) . pack