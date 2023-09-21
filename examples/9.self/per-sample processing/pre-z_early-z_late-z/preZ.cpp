/*
 * 1, PreZ(Zprepass)
 *      实际上就是一个深度计算, PreZ是用一个pass，只算深度
 *      具体用途：
 *          大规模草、透明排序会用到PreZ
 * 2, early-z 和 PreZ的区别
 *      early-z，自动的，对面数有要求，
 *      PreZ，当early-z失效的时候，或者需要深度图的时候，一种手动代替的方案
*/


/**
 * 首先不透明的物体一般需要从前往后渲染，这样可以利用early-z进行z-culling。
 *     Q: 需要对物体进行排序，这样会导致cpu load太重，同时可能会导致没法进行batch。
 *     Q: 同时，如果开启了alpha test等dicard行为，会导致early-z失效；开启了alpha blend等depth diable的行为，也会导致early-z失效。
 *     A: 先进行一个开启alpha test的pre-z pass，写入深度信息，这样相当于自动对所有物体的所有像素进行了排序；
 *        然后进行正常的pass，关闭alpha test，打开深度测试，使用depth equal, 进行渲染。
 * 
 * 其次半透明物体的渲染一般需要从后往前，关闭深度写入，使用alpha blend进行渲染。
 *     Q: 对于一个半透明立方体，可能会出现后面的部分跑到前面的视觉效果，这是因为三角形图元渲染的顺序是不确定的，可能出现后面的三角形晚于前面的三角形渲染，由于没有深度测试，打开了混合，
 *        此时会出现后面的三角形混合前面的三角形。
 *     A: 先用一个pre-z pass写入深度信息，然后pass-2再打开深度测试，depth equal进行渲染，这样会导致立方体的前面和天空盒的混合，后面的三角形被剔除了。
 *     A: 先用一个pass渲染背面，此时会与天空盒混合，然后再用一个pass渲染正面，此时再发生一次混合。
*/


/**
 * Sample: 使用pre-z进行不透明物体的alpha test渲染
*/

int main()
{

}