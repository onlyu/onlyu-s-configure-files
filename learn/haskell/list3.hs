import System.Random
import List2

-- insertAt 'X' "abcd" 2
insertAt :: a -> [a] -> Int -> [a]
insertAt x list pos = first ++ x:rest
  where (first, rest) = splitAt (pos-1) list

range :: Int -> Int -> [Int]
range = enumFromTo

rnd_select xs n = do
  gen <- getStdGen
  return $ take n [ xs !! x | x <- randomRs (0, (length xs) - 1) gen]

diff_select :: Int -> Int -> IO [Int]
diff_select n max = diff_select' n [1..max]

diff_select' _ [] = return []
diff_select' 0 _ = return []
diff_select' n xs = do
  r <- randomRIO (0,(length xs) - 1)
  let (x,y) = removeAt r xs
  rest <- diff_select' (n-1) y
  return (x : rest)

rnd_permu xs = diff_select' (length xs) xs 
  