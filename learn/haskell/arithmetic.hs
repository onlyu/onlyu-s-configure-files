module Arithmetric where

import Test.QuickCheck
import Data.List

-- isPrime

isPrime :: Int -> Bool
isPrime n | n < 2 = False
          | otherwise = ld 2 n == n

ld :: Int -> Int -> Int
ld k n | k^2 > n = n
       | n `rem` k == 0 = k
       | otherwise = ld (k+1) n

myGCD :: Int -> Int -> Int
myGCD a b = myGCD' (min a b) (max a b)
  where
    myGCD' x y | y `rem` x == 0 = x
               | otherwise = myGCD' (y `rem` x) x

coprime :: Int -> Int -> Bool
coprime a b = myGCD a b == 1

totinet 1 = 1
totinet x = totinet' 1 x
  where
    totinet' a b | a >= b = 0
                 | coprime a b = 1 + totinet' (a+1) b
                 | otherwise = totinet' (a+1) b

primes :: Int -> [Int]
primes x = filter isPrime [1..x]

primeFactors :: Int -> [Int]
primeFactors x = primeFactors' (primes x) x
  where
    primeFactors' [] _ = []
    primeFactors' list@(x:xs) y | x > y = []
                                | y `rem` x == 0 = x : primeFactors' list (y `div` x)
                                | otherwise = primeFactors' xs y
                                              
primeFactorMulti :: Int -> [(Int,Int)]
primeFactorMulti = map encode . group . primeFactors
  where
    encode xs = (head xs, length xs)

totinet2 :: Int -> Int
totinet2 m = product [(p - 1) * p ^ (c - 1) | (p, c) <- primeFactorMulti m]

prop_totinet n = n > 0 ==> totinet n == totinet2 n

primesR :: Int -> Int -> [Int]
primesR min max = filter isPrime [min .. max]

goldbach :: Int -> (Int, Int)
goldbach n =
  let a = head . filter (\x -> isPrime x && isPrime (n-x)) . primes $ n
  in (a, n-a)

goldbachList :: Int -> Int -> [(Int, Int)]
goldbachList min max = map goldbach . dropWhile (<4) . filter even $ [min .. max]

goldbachList' :: Int -> Int -> Int -> [(Int, Int)]
goldbachList' min max test = filter ((> test) . fst) $ goldbachList min max
