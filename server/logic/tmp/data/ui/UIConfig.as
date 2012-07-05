package data.ui
{
      public class UIConfig
      {
           [Embed(source = "buzhen.json",mimeType="application/octet-stream")]
           private static var buzhen:Class;
           [Embed(source = "char_property.json",mimeType="application/octet-stream")]
           private static var char_property:Class;
           [Embed(source = "dazuo.json",mimeType="application/octet-stream")]
           private static var dazuo:Class;
           [Embed(source = "ditu_panel.json",mimeType="application/octet-stream")]
           private static var ditu_panel:Class;
           [Embed(source = "down_toolbar.json",mimeType="application/octet-stream")]
           private static var down_toolbar:Class;
           [Embed(source = "edit_panel01.json",mimeType="application/octet-stream")]
           private static var edit_panel01:Class;
           [Embed(source = "edit_panel02.json",mimeType="application/octet-stream")]
           private static var edit_panel02:Class;
           [Embed(source = "edit_panel03.json",mimeType="application/octet-stream")]
           private static var edit_panel03:Class;
           [Embed(source = "edit_panel04.json",mimeType="application/octet-stream")]
           private static var edit_panel04:Class;
           [Embed(source = "equip_prop.json",mimeType="application/octet-stream")]
           private static var equip_prop:Class;
           [Embed(source = "config/FontConfig.json",mimeType="application/octet-stream")]
           private static var FontConfig:Class;
           [Embed(source = "fuben/fubenbiaoti.json",mimeType="application/octet-stream")]
           private static var fuben_fubenbiaoti:Class;
           [Embed(source = "fuben/fubengonglue.json",mimeType="application/octet-stream")]
           private static var fuben_fubengonglue:Class;
           [Embed(source = "fuben/fubenpingjia.json",mimeType="application/octet-stream")]
           private static var fuben_fubenpingjia:Class;
           [Embed(source = "fuben/fubenrukou.json",mimeType="application/octet-stream")]
           private static var fuben_fubenrukou:Class;
           [Embed(source = "fuben/fubenshengli.json",mimeType="application/octet-stream")]
           private static var fuben_fubenshengli:Class;
           [Embed(source = "fuben/fubenshibai.json",mimeType="application/octet-stream")]
           private static var fuben_fubenshibai:Class;
           [Embed(source = "fuben/rukou2.json",mimeType="application/octet-stream")]
           private static var fuben_rukou2:Class;
           [Embed(source = "fuben/rukou_item.json",mimeType="application/octet-stream")]
           private static var fuben_rukou_item:Class;
           [Embed(source = "fuben/yuanbaobuzu.json",mimeType="application/octet-stream")]
           private static var fuben_yuanbaobuzu:Class;
           [Embed(source = "fubenevaluate.json",mimeType="application/octet-stream")]
           private static var fubenevaluate:Class;
           [Embed(source = "fubengonglue.json",mimeType="application/octet-stream")]
           private static var fubengonglue:Class;
           [Embed(source = "fubenleave.json",mimeType="application/octet-stream")]
           private static var fubenleave:Class;
           [Embed(source = "fubenpingjia.json",mimeType="application/octet-stream")]
           private static var fubenpingjia:Class;
           [Embed(source = "getjiangli.json",mimeType="application/octet-stream")]
           private static var getjiangli:Class;
           [Embed(source = "hero_panel.json",mimeType="application/octet-stream")]
           private static var hero_panel:Class;
           [Embed(source = "huobanzhaomu.json",mimeType="application/octet-stream")]
           private static var huobanzhaomu:Class;
           [Embed(source = "huoban_item.json",mimeType="application/octet-stream")]
           private static var huoban_item:Class;
           [Embed(source = "indicator.json",mimeType="application/octet-stream")]
           private static var indicator:Class;
           [Embed(source = "jiangli_panel.json",mimeType="application/octet-stream")]
           private static var jiangli_panel:Class;
           [Embed(source = "jiangli_property.json",mimeType="application/octet-stream")]
           private static var jiangli_property:Class;
           [Embed(source = "missionFollowPanel.json",mimeType="application/octet-stream")]
           private static var missionFollowPanel:Class;
           [Embed(source = "ModuleLoading.json",mimeType="application/octet-stream")]
           private static var ModuleLoading:Class;
           [Embed(source = "qianghua.json",mimeType="application/octet-stream")]
           private static var qianghua:Class;
           [Embed(source = "renwuzhuizong.json",mimeType="application/octet-stream")]
           private static var renwuzhuizong:Class;
           [Embed(source = "renwu_item.json",mimeType="application/octet-stream")]
           private static var renwu_item:Class;
           [Embed(source = "sceneLoading.json",mimeType="application/octet-stream")]
           private static var sceneLoading:Class;
           [Embed(source = "tishi.json",mimeType="application/octet-stream")]
           private static var tishi:Class;
           [Embed(source = "toolBar.json",mimeType="application/octet-stream")]
           private static var toolBar:Class;
           [Embed(source = "up_toolbar.json",mimeType="application/octet-stream")]
           private static var up_toolbar:Class;
           [Embed(source = "warlose.json",mimeType="application/octet-stream")]
           private static var warlose:Class;
           [Embed(source = "warshengli.json",mimeType="application/octet-stream")]
           private static var warshengli:Class;
           [Embed(source = "worldmap.json",mimeType="application/octet-stream")]
           private static var worldmap:Class;
           [Embed(source = "world_map_button.json",mimeType="application/octet-stream")]
           private static var world_map_button:Class;
           [Embed(source = "zhandoushengli.json",mimeType="application/octet-stream")]
           private static var zhandoushengli:Class;
           [Embed(source = "zhaocaicat.json",mimeType="application/octet-stream")]
           private static var zhaocaicat:Class;
           [Embed(source = "zhaomu_item.json",mimeType="application/octet-stream")]
           private static var zhaomu_item:Class;
           [Embed(source = "zhuizong.json",mimeType="application/octet-stream")]
           private static var zhuizong:Class;


           public static function getRes(res:String):Class
           {
                   return UIConfig[res];
           }
      }
}
