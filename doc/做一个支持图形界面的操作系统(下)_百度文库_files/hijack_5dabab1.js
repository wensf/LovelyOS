define("wkview:widget/ui/detectImgBlock/hijack.js",function(i,t,e){var n=i("wkview:widget/ui/detectImgBlock/conf.js").urlHostMap,c=i("wkcommon:widget/ui/js_core/log/log.js"),o=function(){this.init()};o.prototype={init:function(){this.iframeHijack(),this.jsHijack()},iframeHijack:function(){window.self!==window.top&&c.xsend(1,101005,{index:0})},jsHijack:function(){var i=this;setTimeout(function(){for(var t=$("script"),e=$("iframe"),n=$("img"),c=$("a"),o=[t,n,e],a=0;a<o.length;a++)$.each(o[a],function(t,e){var n=e.src;i.whiteListMatch(n)});$.each(c,function(t,e){var n=e.href;i.whiteListMatch(n)})},2500)},whiteListMatch:function(i){var t,e=i;"string"==typeof e&&(e.indexOf(".baidu.com")>-1||e.indexOf("data:image/png;base64")>-1)||e&&(e=e.split("//")[1],t=e&&e.split("/")[0],t&&!n[t]&&c.xsend(1,101005,{index:1,link:i}))}},e.exports=o});