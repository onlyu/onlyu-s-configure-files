-- table (\a b -> (and' a (or' a b)))
import Data.List
import Data.Function
import Data.Ord (comparing)

and' :: Bool -> Bool -> Bool
and' True True = True
and' _ _ = False

or' :: Bool -> Bool -> Bool
or' False False = False
or' _ _ = True

table :: (Bool -> Bool -> Bool) -> [(Bool, Bool, String, Bool)]
table expr = [ (x, y, "=", expr x y) | x <- [True, False], y <- [True, False]]

gray 1 = ["0", "1"]
gray n = [ x : xs | x <- ['0', '1'], xs <- gray (n-1)]

type Code = Char
type Frequence = Int
data HuffmanTree = Leaf Code Frequence
                 | Branch HuffmanTree HuffmanTree Frequence
                   deriving Show

buildHuffmanTree :: [(Code, Frequence)] -> HuffmanTree
buildHuffmanTree = mergeHuffmanTree . sortBy (comparing getFrequence) . map (\x -> Leaf (fst x) (snd x))

mergeHuffmanTree :: [HuffmanTree] -> HuffmanTree
mergeHuffmanTree [] = error "merge empty tree"
mergeHuffmanTree [x] = x
mergeHuffmanTree (x:y:xys) = mergeHuffmanTree $ insertBy (comparing getFrequence) (Branch x y (getFrequence x + getFrequence y)) xys

getFrequence :: HuffmanTree -> Frequence
getFrequence (Leaf _ x) = x
getFrequence (Branch _ _ x) = x

codeTable :: HuffmanTree -> [(Code, String)]
codeTable = codeTable' ""
  where
    codeTable' history (Leaf c _) = [(c, history)]
    codeTable' history (Branch l r _) = codeTable' (history ++ "0") l ++ codeTable' (history ++ "1") r

huffman :: [(Code, Frequence)] -> [(Code, String)]
huffman = sortBy (comparing fst) . codeTable . buildHuffmanTree

