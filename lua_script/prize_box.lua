--装载配置表
local goods = require "prize_box_config"
local prize_box = {}
--[[
概要:
1.必须将xkey 奖项表存为全局变量, 每次抽奖之后都需要用到这个表去查实际中奖的物品是什么
2.必须将box_table 抽奖池存为全局变量, 每次抽奖都必须从池里面随机踏点
3.由于算法设计, 你也必须将抽奖池的总量len 也存为全局变量
4.概率表xval 是临时表(记录抽奖的中奖次数), 但是在box_table 抽奖池没有建好之前, 这个表都不能被修改

5.如果你需要做必种算法, 简单... 重新填写一份配置, 形式差不多一样的, 但是你需要(最好)重新保存一下以上的1,2,3,4 条规则里面的
变量如: box_table 抽奖池, xkey 奖项表, 抽奖池的总量len... 然而每次抽奖还是同样调用do_prize_draw, 你传入抽奖池和池长度即可
兑现奖品也是一样用get_goods_name

6.执行此模块必须先调用初始化函数init_prize_box
]]
--抽奖池1(这些变量都是空的table, 用来保存init_prize_box 的返回值)
local xkey1,xval1;
local box1,box_count1;
--注意: lua table 不能像cpp 那样进行指针引用, 所以这里适合直接返回整个table 变量(返回时已经赋了很多值)... 这时直接用个指针接住即可
--如果你想传入一个指针, 然后给指针赋值, 在lua 里面是不被推荐的


--遍历goods 将key and value 拆开, 返回两个表: 物品表和概率表(并行table-不能重新进行排序), 一个奖项个数总和
--注意: 如果没有发现负值or 没有屁股会报错（返回两个nil）
local function break_off(goods)--这个函数在模块的初始化期间运行, 性能并不是首要的关键
  --正确性校验
  local tmp = 0;
  local count = 0;
  local len = 0;
  for key, value in pairs(goods) do
    if value < 0 then print("key",key,"为负值!!"); return nil,nil; end
    if value == 0 then tmp = tmp + 1; end
    count = count + value;
    len = len + 1;
  end
  if tmp == 0 then print"没有屁股"; return nil,nil; end
  if tmp ~= 1 then print"屁股数量不正确"; return nil,nil; end
  
  print("奖项个数:", len, "除屁股外所有物品的中奖概率总和:", count);
  
  local xkey = {};
  local xval = {};
  local xx_tmp = 1;
  for key, value in pairs(goods) do--拆分
    xkey[xx_tmp] = key;
    xval[xx_tmp] = value;
    xx_tmp = xx_tmp + 1;
    --table.insert(xkey, key);
    --table.insert(xval, value);
  end
  
  if #xkey == len and #xval == len then
    return xkey, xval;
  else
    print "未知错误, #xkey == len and #xval == len 不成立!!";
    return nil,nil;
  end
end

--
--构建抽奖池--break_off 的结果传入...成功返回box_table,box_count, 失败返回nil,0
local _modf = math.modf;
local function make_prize_box(xkey, xval)
  --求出xval 最小的值
  local min = xval[1];
  for i=2,#xval do
    if xval[i] < min and xval[i] ~= 0 then--不能取屁股值
      min = xval[i];
    end
  end
  --只要还有小数部分, 都要继续乘以10, 直到所有小数位为0
  local xx = 1;
  local x1,x0;
  while(true)do
    x1,x0 = math.modf(min);--拆分最小值的整数和小数, 直到最小值变成没有小数的数
    if x0 ~= 0 then
      xx = xx * 10;
      min = min * 10;
    else
      break;
    end
  end
  --lua 的math.modf 函数不稳定, 为了防止有bug 撑爆服务器内存!!超过百万份之一需要修改这行代码才能获得权限
  --local x1,x0 = math.modf(123014.30);print(x1,x0);
  --local x1,x0 = math.modf(123.01430);print(x1,x0);--形如这样的分拆, 总是出问题
  if xx > 1000000 then
    return nil,0;
  end
  
  local count = 0;--除了屁股外所有奖项的份额
  --所有值都去掉小数位
  for i=1,#xval do xval[i] = xval[i] * xx; count = count + xval[i]; end
  --开始填池
  local box = {};
  local box_pos = 1;
  for i=1,#xval do
    if xval[i] ~= 0 then
      for x=1,xval[i] do
        box[box_pos] = i;
        box_pos = box_pos + 1;
        --table.insert(box,i);--以key 为权值, 抽奖后的值, 也可以在xkey 中直接索引出奖品是什么
      end
    else
      count = xx - count;--计算屁股的数量
      for x=1,count do
        box[box_pos] = i;
        box_pos = box_pos + 1;
        --table.insert(box,i);--屁股仍然会遵守key=i 的原则
      end
    end
  end

  return box,xx;
end
--
--执行抽奖
local _random = math.random
local _time = os.time
function prize_box.do_prize_draw(box, count)
  --local random = math.random(os.time())%count+1;
  local random = _random(_time())%count+1;
  return box[random];
end
--
--判断是什么奖品
function prize_box.get_goods_name(xkey, code)
  return xkey[code];
end

--自动模块组建, 上面的函数是零散的, 这个是组建抽奖模块的过程, 不能省略
--初始化函数--成功返回0, 失败返回-1
function prize_box.init_prize_box()
  local xkey, xval, box, box_count;
  xkey, xval = break_off(goods);
  if xkey ~= nil and xval ~= nil then
    box,box_count = make_prize_box(xkey, xval);
    if box ~= nil and box_count ~= 0 then
      --打印出权值
      local ttmp = 0;
      for i=1,#xval do print(xval[i]); ttmp = ttmp + xval[i]; end
      print(box_count - ttmp);
      return xkey, xval, box, box_count;
    end
  else
    return nil,nil,nil,0;
  end
end

--测试函数(如果你需要修改, 那么可以去修改奖项配置表)
local function test(xkey, xval, box, box_count)
  --10w 次抽奖测试
  local xstart = os.time();
  local tmp;
  local xprize;
  
  --概率表xval 已经没用了, 但是可以做一个很好的中奖次数统计器
  for i=1,#xval do xval[i] = 0; end
  
  --10w 次抽奖测试
  for i=1,100000 do
    tmp = prize_box.do_prize_draw(box,box_count);
    xprize = prize_box.get_goods_name(xkey,tmp);--得出奖品
    if xprize == nil then
      print("抽奖模块设计有问题, 10w 测试失败",tmp,xprize,#box,box_count); return;
    end
    
    xval[tmp] = xval[tmp] + 1;--累加for 统计
    
    print(xprize);--打印奖品, 考虑性能时不打印
  end
  
  --测试完毕显示统计信息(方便你调试抽奖算法的平衡性)
  for i=1,#xkey do
    print("奖项:",xkey[i],"中奖累计:",xval[i]);
  end
  
  local xend = os.time();
  print("10w 次抽奖操作总耗时",xend - xstart);--耗时统计
end



--执行测试任务(例子程序, 调用时可参照, 运行时去掉)
xkey1, xval1, box1, box_count1 = prize_box.init_prize_box();--初始化抽奖池1
if xkey1 ~= nil and xval1 ~= nil and box1 ~= nil and box_count1 ~= 0 then
  print(#xkey1, #xval1, #box1, box_count1);
else
  print"初始化抽奖池失败～～～!";
end

test(xkey1, xval1, box1, box_count1);--测试抽奖池1



