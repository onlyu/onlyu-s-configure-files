import List1

-- Modified run-length encoding.
data ListItem a = Single a | Multiple Int a
                deriving (Show)
                         
encodeModified :: Eq a => [a] -> [ListItem a]
encodeModified = map encodeHelper . encode
  where
    encodeHelper (1,x) = Single x
    encodeHelper (n,x) = Multiple n x
    
decodeModified :: [ListItem a] -> [a]
decodeModified = concatMap decodeItem
  where
    decodeItem (Single x) = [x]
    decodeItem (Multiple x y) = replicate x y

-- Duplicate the elements of a list.
dupli :: [a] -> [a]
dupli = concatMap (replicate 2)

repli :: [a] -> Int -> [a]
repli list num = concatMap (replicate num) list

dropEvery :: [a] -> Int -> [a]
dropEvery list num
  | length list > num = (take (num - 1) list) ++ (dropEvery (drop num list) num)
  | otherwise = list

split :: [a] -> Int -> ([a],[a])
split list wh = (take wh list, drop wh list)

slice :: [a] -> Int -> Int -> [a]
slice list begin end = take (end - begin + 1) $ drop (begin - 1) list

rotate :: [a] -> Int -> [a]
rotate list num = (drop left list) ++ (take left list)
  where
    left = if num >= 0 then num else num + (length list)

removeAt :: Int -> [a] -> (a, [a])
removeAt pos list = case rest of
	[] -> error "removeAt: index too large"
        x:xs -> (x, first ++ xs)
  where (first,rest) = splitAt pos list
