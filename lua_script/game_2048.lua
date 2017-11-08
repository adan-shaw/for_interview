print("game named 2048...power by adan shaw - 2017/09/08");

print("control: ");
print("direction: w = up");
print("direction: s = down");
print("direction: a = left");
print("direction: d = right");
print("exit     : exit");
print("");

print("rules: ");
print("every move system will add a new number 2 or 4 in the two-dimensional table, and pos is random");
print("first get added number 2048 win");
print("");

--游戏板(全局变量-但不增长的table)
--mother = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};--标准启动

--测试数据
--mother = {2,4,8,16, 32,64,128,256, 512,1024,1024,512, 256,128,64,0};--赢or 输
mother = {2,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,0};--特殊1-每步只消除1
--mother = {2,0,0,2, 0,0,0,0, 0,0,0,2, 2,0,0,0};--特殊2-间隔融合测试
--mother = {2,2,2,0, 0,0,0,0, 2,0,0,0, 2,0,0,0};--特殊3







--更新显示(无返回)
local _print = print;
function update_show()
  _print(mother[1],mother[2],mother[3],mother[4]);
  _print(mother[5],mother[6],mother[7],mother[8]);
  _print(mother[9],mother[10],mother[11],mother[12]);
  _print(mother[13],mother[14],mother[15],mother[16]);
  _print("");
end
update_show()

--判断赢(赢 = 0, 游戏继续 = -1, -2 为未知错误)
function is_win()
  --防止游戏崩溃
  local len = #mother;
  if len ~= 16 then print("unknow error, table len is wrong!!", len); update_show(); return -2; end
  
  --检查是否胜利(胜利优先, 防止最后一次操作时刚好满格, 但是又刚好爆出2048)
  for i=1,len do
    if mother[i] >=2048 then print "congratulations!! you win"; return 0; end
  end

  return -1;--继续游戏
end


--消除一列(每次只能消除一次, 不能一消再消, 那就是不用迭代了)--无返回
function kill_number(x, col)
  if x == 1 then--up
    if col == 1 then
      --消牌--防止0101 or 1001 这样的隔牌消牌, 先重排,再消牌, 再重排...如果是: 1122型, 消牌之后会是0101型, 同样需要重排3次...别想省逻辑
      for c=1,3 do for i=9,1,-4 do if mother[i] == 0 and mother[i+4] ~= 0 then mother[i] = mother[i+4]; mother[i+4] = 0; end end end
      
      if mother[1] == mother[5] and mother[1] ~= 0 then mother[1] = mother[1]*2; mother[5] = 0; if mother[9] == mother[13] and mother[9] ~= 0 then mother[9] = mother[9]*2; mother[13] = 0; end end
      if mother[5] == mother[9] and mother[5] ~= 0 then mother[5] = mother[5]*2; mother[9] = 0; else if mother[9] == mother[13] and mother[9] ~= 0 then mother[9] = mother[9]*2; mother[13] = 0; end end
      --无论如何也会对牌进行重排列--防止0 1 0 1 left and 0 1 1 1 left 这样的错误重排, 再执行2次, 一共三次
      for c=1,3 do for i=9,1,-4 do if mother[i] == 0 and mother[i+4] ~= 0 then mother[i] = mother[i+4]; mother[i+4] = 0; end end end
      return;
    end
    if col == 2 then
      for c=1,3 do for i=10,2,-4 do if mother[i] == 0 and mother[i+4] ~= 0 then mother[i] = mother[i+4]; mother[i+4] = 0; end end end
      if mother[2] == mother[6] and mother[2] ~= 0 then mother[2] = mother[2]*2; mother[6] = 0; if mother[10] == mother[14] and mother[10] ~= 0 then mother[10] = mother[10]*2; mother[14] = 0; end end
      if mother[6] == mother[10] and mother[6] ~= 0 then mother[6] = mother[6]*2; mother[10] = 0; else if mother[10] == mother[14] and mother[10] ~= 0 then mother[10] = mother[10]*2; mother[14] = 0; end end
      for c=1,3 do for i=10,2,-4 do if mother[i] == 0 and mother[i+4] ~= 0 then mother[i] = mother[i+4]; mother[i+4] = 0; end end end
      return;
    end
    if col == 3 then
      for c=1,3 do for i=11,3,-4 do if mother[i] == 0 and mother[i+4] ~= 0 then mother[i] = mother[i+4]; mother[i+4] = 0; end end end
      if mother[3] == mother[7] and mother[3] ~= 0 then mother[3] = mother[3]*2; mother[7] = 0; if mother[11] == mother[15] and mother[11] ~= 0 then mother[11] = mother[11]*2; mother[15] = 0; end end
      if mother[7] == mother[11] and mother[7] ~= 0 then mother[7] = mother[7]*2; mother[11] = 0; else if mother[11] == mother[15] and mother[11] ~= 0 then mother[11] = mother[11]*2; mother[15] = 0; end end
      for c=1,3 do for i=11,3,-4 do if mother[i] == 0 and mother[i+4] ~= 0 then mother[i] = mother[i+4]; mother[i+4] = 0; end end end
      return;
    end
    if col == 4 then
      for c=1,3 do for i=12,4,-4 do if mother[i] == 0 and mother[i+4] ~= 0 then mother[i] = mother[i+4]; mother[i+4] = 0; end end end
      if mother[4] == mother[8] and mother[4] ~= 0 then mother[4] = mother[4]*2; mother[8] = 0; if mother[12] == mother[16] and mother[12] ~= 0 then mother[12] = mother[12]*2; mother[16] = 0; end end
      if mother[8] == mother[12] and mother[8] ~= 0 then mother[8] = mother[8]*2; mother[12] = 0; else if mother[12] == mother[16] and mother[12] ~= 0 then mother[12] = mother[12]*2; mother[16] = 0; end end
      for c=1,3 do for i=12,4,-4 do if mother[i] == 0 and mother[i+4] ~= 0 then mother[i] = mother[i+4]; mother[i+4] = 0; end end end
      return;
    end
  end

  if x == 2 then--down
    if col == 1 then
      for c=1,3 do for i=5,13,4 do if mother[i] == 0 and mother[i-4] ~= 0 then mother[i] = mother[i-4]; mother[i-4] = 0; end end end
      if mother[9] == mother[13] and mother[9] ~= 0 then mother[13] = mother[13]*2; mother[9] = 0; if mother[1] == mother[5] and mother[1] ~= 0 then mother[5] = mother[5]*2; mother[1] = 0; end end
      if mother[5] == mother[9] and mother[5] ~= 0 then mother[9] = mother[9]*2; mother[5] = 0; else if mother[1] == mother[5] and mother[1] ~= 0 then mother[5] = mother[5]*2; mother[1] = 0; end end
      for c=1,3 do for i=5,13,4 do if mother[i] == 0 and mother[i-4] ~= 0 then mother[i] = mother[i-4]; mother[i-4] = 0; end end end
      return;
    end
    if col == 2 then
      for c=1,3 do for i=6,14,4 do if mother[i] == 0 and mother[i-4] ~= 0 then mother[i] = mother[i-4]; mother[i-4] = 0; end end end
      if mother[10] == mother[14] and mother[10] ~= 0 then mother[14] = mother[14]*2; mother[10] = 0; if mother[2] == mother[6] and mother[2] ~= 0 then mother[6] = mother[6]*2; mother[2] = 0; end end
      if mother[6] == mother[10] and mother[6] ~= 0 then mother[10] = mother[10]*2; mother[6] = 0; else if mother[2] == mother[6] and mother[2] ~= 0 then mother[6] = mother[6]*2; mother[2] = 0; end end
      for c=1,3 do for i=6,14,4 do if mother[i] == 0 and mother[i-4] ~= 0 then mother[i] = mother[i-4]; mother[i-4] = 0; end end end
      return;
    end
    if col == 3 then
      for c=1,3 do for i=7,15,4 do if mother[i] == 0 and mother[i-4] ~= 0 then mother[i] = mother[i-4]; mother[i-4] = 0; end end end
      if mother[11] == mother[15] and mother[11] ~= 0 then mother[15] = mother[15]*2; mother[11] = 0; if mother[3] == mother[7] and mother[3] ~= 0 then mother[7] = mother[7]*2; mother[3] = 0; end end
      if mother[7] == mother[11] and mother[7] ~= 0 then mother[11] = mother[11]*2; mother[7] = 0; else if mother[3] == mother[7] and mother[3] ~= 0 then mother[7] = mother[7]*2; mother[3] = 0; end end
      for c=1,3 do for i=7,15,4 do if mother[i] == 0 and mother[i-4] ~= 0 then mother[i] = mother[i-4]; mother[i-4] = 0; end end end
      return;
    end
    if col == 4 then
      for c=1,3 do for i=8,16,4 do if mother[i] == 0 and mother[i-4] ~= 0 then mother[i] = mother[i-4]; mother[i-4] = 0; end end end
      if mother[12] == mother[16] and mother[12] ~= 0 then mother[16] = mother[16]*2; mother[12] = 0; if mother[4] == mother[8] and mother[4] ~= 0 then mother[8] = mother[8]*2; mother[4] = 0; end end
      if mother[8] == mother[12] and mother[8] ~= 0 then mother[12] = mother[12]*2; mother[8] = 0; else if mother[4] == mother[8] and mother[4] ~= 0 then mother[8] = mother[8]*2; mother[4] = 0; end end
      for c=1,3 do for i=8,16,4 do if mother[i] == 0 and mother[i-4] ~= 0 then mother[i] = mother[i-4]; mother[i-4] = 0; end end end
      return;
    end
  end

  if x == 3 then--left
    if col == 1 then
      for c=1,3 do for i=3,1,-1 do if mother[i] == 0 and mother[i+1] ~= 0 then mother[i] = mother[i+1]; mother[i+1] = 0; end end end
      if mother[1] == mother[2] and mother[1] ~= 0 then mother[1] = mother[1]*2; mother[2] = 0; if mother[3] == mother[4] and mother[3] ~= 0 then mother[3] = mother[3]*2; mother[4] = 0; end end
      if mother[2] == mother[3] and mother[2] ~= 0 then mother[2] = mother[2]*2; mother[3] = 0; else if mother[3] == mother[4] and mother[3] ~= 0 then mother[3] = mother[3]*2; mother[4] = 0; end end
      for c=1,3 do for i=3,1,-1 do if mother[i] == 0 and mother[i+1] ~= 0 then mother[i] = mother[i+1]; mother[i+1] = 0; end end end
      return;
    end
    if col == 2 then
      for c=1,3 do for i=7,5,-1 do if mother[i] == 0 and mother[i+1] ~= 0 then mother[i] = mother[i+1]; mother[i+1] = 0; end end end
      if mother[5] == mother[6] and mother[5] ~= 0 then mother[5] = mother[5]*2; mother[6] = 0; if mother[7] == mother[8] and mother[7] ~= 0 then mother[7] = mother[7]*2; mother[8] = 0; end end
      if mother[6] == mother[7] and mother[6] ~= 0 then mother[6] = mother[6]*2; mother[7] = 0; else if mother[7] == mother[8] and mother[7] ~= 0 then mother[7] = mother[7]*2; mother[8] = 0; end end
      for c=1,3 do for i=7,5,-1 do if mother[i] == 0 and mother[i+1] ~= 0 then mother[i] = mother[i+1]; mother[i+1] = 0; end end end
      return;
    end
    if col == 3 then
      for c=1,3 do for i=11,9,-1 do if mother[i] == 0 and mother[i+1] ~= 0 then mother[i] = mother[i+1]; mother[i+1] = 0; end end end
      if mother[9] == mother[10] and mother[9] ~= 0 then mother[9] = mother[9]*2; mother[10] = 0; if mother[11] == mother[12] and mother[11] ~= 0 then mother[11] = mother[11]*2; mother[12] = 0; end end
      if mother[10] == mother[11] and mother[10] ~= 0 then mother[10] = mother[10]*2; mother[11] = 0; else if mother[11] == mother[12] and mother[11] ~= 0 then mother[11] = mother[11]*2; mother[12] = 0; end end
      for c=1,3 do for i=11,9,-1 do if mother[i] == 0 and mother[i+1] ~= 0 then mother[i] = mother[i+1]; mother[i+1] = 0; end end end
      return;
    end
    if col == 4 then
      for c=1,3 do for i=15,13,-1 do if mother[i] == 0 and mother[i+1] ~= 0 then mother[i] = mother[i+1]; mother[i+1] = 0; end end end
      if mother[13] == mother[14] and mother[13] ~= 0 then mother[13] = mother[13]*2; mother[14] = 0; if mother[15] == mother[16] and mother[15] ~= 0 then mother[15] = mother[15]*2; mother[16] = 0; end end
      if mother[14] == mother[15] and mother[14] ~= 0 then mother[14] = mother[14]*2; mother[15] = 0; else if mother[15] == mother[16] and mother[15] ~= 0 then mother[15] = mother[15]*2; mother[16] = 0; end end
      for c=1,3 do for i=15,13,-1 do if mother[i] == 0 and mother[i+1] ~= 0 then mother[i] = mother[i+1]; mother[i+1] = 0; end end end
      return;
    end
  end

  if x == 4 then--right
    if col == 1 then
      for c=1,3 do for i=2,4 do if mother[i] == 0 and mother[i-1] ~= 0 then mother[i] = mother[i-1]; mother[i-1] = 0; end end end
      if mother[3] == mother[4] and mother[3] ~= 0 then mother[4] = mother[4]*2; mother[3] = 0; if mother[1] == mother[2] and mother[1] ~= 0 then mother[2] = mother[2]*2; mother[1] = 0; end end
      if mother[2] == mother[3] and mother[2] ~= 0 then mother[3] = mother[3]*2; mother[2] = 0; else if mother[1] == mother[2] and mother[1] ~= 0 then mother[2] = mother[2]*2; mother[1] = 0; end end
      for c=1,3 do for i=2,4 do if mother[i] == 0 and mother[i-1] ~= 0 then mother[i] = mother[i-1]; mother[i-1] = 0; end end end
      return;
    end
    if col == 2 then
      for c=1,3 do for i=6,8 do if mother[i] == 0 and mother[i-1] ~= 0 then mother[i] = mother[i-1]; mother[i-1] = 0; end end end
      if mother[7] == mother[8] and mother[7] ~= 0 then mother[8] = mother[8]*2; mother[7] = 0; if mother[5] == mother[6] and mother[5] ~= 0 then mother[6] = mother[6]*2; mother[5] = 0; end end
      if mother[6] == mother[7] and mother[6] ~= 0 then mother[7] = mother[7]*2; mother[6] = 0; else if mother[5] == mother[6] and mother[5] ~= 0 then mother[6] = mother[6]*2; mother[5] = 0; end end
      for c=1,3 do for i=6,8 do if mother[i] == 0 and mother[i-1] ~= 0 then mother[i] = mother[i-1]; mother[i-1] = 0; end end end
      return;
    end
    if col == 3 then
      for c=1,3 do for i=10,12 do if mother[i] == 0 and mother[i-1] ~= 0 then mother[i] = mother[i-1]; mother[i-1] = 0; end end end
      if mother[11] == mother[12] and mother[11] ~= 0 then mother[12] = mother[12]*2; mother[11] = 0; if mother[9] == mother[10] and mother[9] ~= 0 then mother[10] = mother[10]*2; mother[9] = 0; end end
      if mother[10] == mother[11] and mother[10] ~= 0 then mother[11] = mother[11]*2; mother[10] = 0; else if mother[9] == mother[10] and mother[9] ~= 0 then mother[10] = mother[10]*2; mother[9] = 0; end end
      for c=1,3 do for i=10,12 do if mother[i] == 0 and mother[i-1] ~= 0 then mother[i] = mother[i-1]; mother[i-1] = 0; end end end
      return;
    end
    if col == 4 then
      for c=1,3 do for i=14,16 do if mother[i] == 0 and mother[i-1] ~= 0 then mother[i] = mother[i-1]; mother[i-1] = 0; end end end
      if mother[15] == mother[16] and mother[15] ~= 0 then mother[16] = mother[16]*2; mother[15] = 0; if mother[13] == mother[14] and mother[13] ~= 0 then mother[14] = mother[14]*2; mother[13] = 0; end end
      if mother[14] == mother[15] and mother[14] ~= 0 then mother[15] = mother[15]*2; mother[14] = 0; else if mother[13] == mother[14] and mother[13] ~= 0 then mother[14] = mother[14]*2; mother[13] = 0; end end
      for c=1,3 do for i=14,16 do if mother[i] == 0 and mother[i-1] ~= 0 then mother[i] = mother[i-1]; mother[i-1] = 0; end end end
      return;
    end
  end
end

--随机填数(game over = 0, 继续游戏-1)
local _random = math.random
local _time = os.time
function fall_number()
  --检查是否输了
  local len = #mother;
  if len ~= 16 then print("unknow error, table len is wrong!!", len); update_show(); return 0; end
  local tmp = 0;--初始化
  for i=1,len do
    if mother[i] ~= 0 then tmp = tmp + 1; end
  end
  if tmp == 16 then print "sad!! you losed this game"; return 0; end--已输
  --
  --首先确认mother 里面至少含有一个0, 否则会陷入死循环...但是实际情况下, 这种情况不太可能碰到, 测试的时候, 如果你要填满数据, 请先将初始化的一个随机数去掉
  --如果不填满数据, 你又想开局就输, 那么, 你可以留下一个0, 周边的都是32 64 之类完全不可能相容的, 随机第一个数就结束游戏
  while(true) do
    local rand = _random(_time())%16+1;
    --print("rand",rand);--for test
    if mother[rand] == 0 then
      if rand%2 == 0 then
        mother[rand] = 2;return -1;
      else
        mother[rand] = 4;return -1;
      end
    end
  end
  print "unknow error in fall_number!!"
  return 0;
end

--计算逻辑(传入参数x, 根据x 来操作上下左右叠加)++(game over = 0, 继续游戏-1)
--消牌原则: 顶部蹦叠 or 底部蹦叠 or 中间开始蹦叠 -- 不管！
--只比较方向 + 相邻的两个数是否相等, 相等就方向叠加一个, 然后清0一个, 然后递归--剔除0
function math_2048(x)
  local t1,t2,t3,t4
  local tmp = 0
  if x == 1 or x == 2 then--up or down
    t1 = mother[1] + mother[5] + mother[9] + mother[13];
    t2 = mother[2] + mother[6] + mother[10] + mother[14];
    t3 = mother[3] + mother[7] + mother[11] + mother[15];
    t4 = mother[4] + mother[8] + mother[12] + mother[16];
    if t1 ~= 0 then kill_number(x, 1); end
    if t2 ~= 0 then kill_number(x, 2); end
    if t3 ~= 0 then kill_number(x, 3); end
    if t4 ~= 0 then kill_number(x, 4); end
    tmp = fall_number();--随机填牌(如果消完牌还有16个, 则无牌可填, game over)
    return tmp;
  end

  if x == 3 or x == 4 then--left or right
    t1 = mother[1] + mother[2] + mother[3] + mother[4];
    t2 = mother[5] + mother[6] + mother[7] + mother[8];
    t3 = mother[9] + mother[10] + mother[11] + mother[12];
    t4 = mother[13] + mother[14] + mother[15] + mother[16];
    if t1 ~= 0 then kill_number(x, 1); end
    if t2 ~= 0 then kill_number(x, 2); end
    if t3 ~= 0 then kill_number(x, 3); end
    if t4 ~= 0 then kill_number(x, 4); end
    tmp = fall_number();
    return tmp;
  end

  print "unknow error, in math_2048(x), killing software !!";
  table.insert(mother,0);--伪造游戏崩溃信息
  return 0;
end


--输入x(返回-1 继续, 返回0 退出)
local _read = io.read
function input_x()
  local x = _read();
  print "";
  local xx = -1
  if x == "w" then--up
    xx = math_2048(1);--计算操作结果
    x = is_win();--判断是否胜利
    if x == -1 and xx == -1 then return -1; else return 0; end
  end

  if x == "s" then--down
    xx = math_2048(2);
    x = is_win();
    if x == -1 and xx == -1 then return -1; else return 0; end
  end

  if x == "a" then--left
    xx = math_2048(3);
    x = is_win();
    if x == -1 and xx == -1 then return -1; else return 0; end
  end

  if x == "d" then--right
    xx = math_2048(4);
    x = is_win();
    if x == -1 and xx == -1 then return -1; else return 0; end
  end
  
  if x == "exit" then
    x = is_win();
    return 0;
  end
 
  print("you are inputted a number!! ");
  print("direction: w = up");
  print("direction: s = down");
  print("direction: a = left");
  print("direction: d = right");
  print("exit     : exit");
  print("");
  --update_show();
  return -1;--输入错误继续
end



--主函数
function main()
  fall_number();--初始化一个随机number
  update_show();--显示第一波数字
  local tmp = -1;
  while(true) do
    tmp = input_x();
    if tmp == 0 then break; else update_show(); end
  end
  update_show();
  print "game over !!";
end

--调用主函数
main();

