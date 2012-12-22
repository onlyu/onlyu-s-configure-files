

import Data.List

-- Find the last element of a list.
myLast :: [a] -> a
myLast [x] = x
myLast (x:xs) = myLast xs

-- Find the last but one element of a list.
myButLast :: [a] -> a
myButLast [x, _] = x
myButLast (x:xs) = myButLast xs

-- Find the K'th element of a list. The first element in the list is number 1.
elementAt :: [a] -> Int -> a
elementAt (x:xs) 1 = x
elementAt (x:xs) n = elementAt xs (n-1)

-- Find the number of elements of a list.
myLength :: [a] -> Int
myLength [] = 0
myLength (x:xs) = 1 + myLength xs

-- Reverse a list.
myReverse :: [a] -> [a]
myReverse [] = []
myReverse (x:xs) = myReverse xs ++ [x]

-- Find out whether a list is a palindrome. A palindrome can be read forward or backward; e.g. (x a m a x).
isPalindrome :: Eq a => [a] -> Bool
isPalindrome list = listEq list (myReverse list)
  where
    listEq [] [] = True
    listEq [] _ = False
    listEq _ [] = False
    listEq (x:xs) (y:ys) = (x == y) && listEq xs ys

-- Flatten a nested list structure.
data NestedList a = Elem a | List [NestedList a]
myFlatten :: NestedList a -> [a]
myFlatten (Elem a) = [a]
myFlatten (List []) = []
myFlatten (List (x:xs)) = myFlatten x ++ myFlatten (List xs)

-- Eliminate consecutive duplicates of list elements.
myCompress :: Ord a => [a] -> [a]
myCompress list = myReverse $ compress (sort list) []
  where
    compress [] b = b
    compress (x:xs) [] = compress xs [x]
    compress (x:xs) b@(y:ys) = if x == y then
                                 compress xs b
                               else
                                 compress xs (x:b)


pack :: Ord a => [a] -> [[a]]
pack list = myReverse $ doPack (sort list) []
  where
    doPack [] b = b
    doPack (x:xs) [] = doPack xs [[x]]
    doPack (x:xs) list@((y:ys):zs) =
      if x == y then
        doPack xs ((x:y:ys):zs)
      else
        doPack xs ([x]:list)

encode :: Ord a => [a] -> [(Int,a)]
encode = (map (\x -> (length x, head x))) . pack