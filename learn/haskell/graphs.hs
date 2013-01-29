import Data.List

type Edge = (Int, Int)
type Graph = [Edge]
type Path = [Int]


-- this is comment
paths :: Int -> Int -> Graph -> [Path]
paths _ _ [] = [[]]
paths start end graph
    | start == end = [[end]]
    | otherwise = map (start:) $ concatMap paths' success
    where
        paths' start = paths start end rest
        rest = filter ((/= start) . fst) graph
        success = map snd $ graph \\ rest

-- problem 82
cycle :: Int -> Graph -> [Path]
cycle x graph = map (x:) $ concatMap (\f -> f graph) paths'
    where
        paths' = map ((\s -> paths s x) . snd) $ filter ((== x) . fst) graph
