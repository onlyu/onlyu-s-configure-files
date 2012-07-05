
#校验结果的代码，待补充

#uid数量校验，有110和119重复
#看玩家的帮派信息
find . -name "u*.py"|xargs grep "mOrgInfo" |awk -F "mOrgInfo" '{print $NF}'|awk -F "}" '{print $1}'|sort |uniq -c|sort -rn

#校验
find ./100* -name "u*.py"|xargs grep "mOrgInfo"|awk -F "mOrgInfo" '{print $NF}'|awk -F "}" '{print $1}'|sort |uniq -c|sort -rn|more

find ./user -name "u*.py"|xargs grep "mOrgInfo"|awk -F "mOrgInfo" '{print $NF}'|awk -F "}" '{print $1}'|sort |uniq -c|sort -rn|more  
