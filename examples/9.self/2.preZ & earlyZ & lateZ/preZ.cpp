/*
 * 1, PreZ(Zprepass)
 *  实际上就是一个深度计算
 *  和深度图的区别
 *  都是深度信息
 *  PreZ是用一个pass，只算深度
 *  深度图是算成了一张RT，把深度信息绘制到了一张RT上。
 *  具体用途：
 *   大规模草、透明排序会用到PreZ
 * 2, early-z 和 PreZ的区别
 *  early-z，自动的，对面数有要求，
 *  PreZ，当early-z失效的时候，或者需要深度图的时候，一种手动代替的方案
*/