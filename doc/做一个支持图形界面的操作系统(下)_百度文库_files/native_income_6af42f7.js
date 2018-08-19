define("wkview:widget/raw_income/native_income/native_income.js",function(a,t,i){var s=a("wkcommon:widget/ui/lib/jquery/jquery.js"),e=(a("wkcommon:widget/lib/tangram/base/base.js"),a("wkcommon:widget/ui/js_core/_dialog/_dialog.js")),n=a("wkcommon:widget/lib/doT/doT.min.js"),c=a("wkview:widget/raw_income/raw_income.js"),o=a("wkcommon:widget/ui/js_core/log/log.js"),d=(a("wkview:widget/html_view/html_reader/js/page.js").page,a("wkcommon:widget/ui/js_core/xform/xform.js")),l=(a("wkcommon:widget/ui/lib/jquery_xdomainrequest/xdomainrequest.js"),function(){this.option={},this.cacheList="",this.video="",this.cacheData={},this.downloadTpl=n.template('<!-- htmlcs-disable -->\n{{? it.list == 1 }}\n<div class="ads-doc-list">\n    <div class="header">\n        <div class="myclose"></div>\n        <div class="close list-close-btn"></div>\n        <h2>{{= it.data.title}}</h2>\n        <p>\u5171<span class="counter">{{= it.data.doc_list.length}}</span>\u7bc7\u6587\u6863&nbsp;&nbsp;{{= it.data.count}}\u4eba\u5df2\u4e0b\u8f7d</p>\n        </div>\n    <div class="cont" id="cont">\n        <ul class="list dialog-list">\n            {{if (it.data && it.data.doc_list.length >0) {}}\n            {{for(var i=0;i< it.data.doc_list.length;i++) {}}\n            <li><span class="number">{{=i+1}}</span> <span class="ic ic-{{=it.data.doc_list[i].type}}"></span>{{=it.data.doc_list[i].title}}</li>\n            {{}}}\n            {{ } }}\n        </ul>\n        <a href="###" class="ui-bz-btn-senior native-ads-download-btn">\n            <b class="ui-bz-btn-p-40 ui-bz-btc ">\u514d\u8d39\u4e0b\u8f7d</b>\n            </a>\n        </div>\n    </div>\n{{?? it.list == 0}}\n<div class="download-success-dialog">\n    <div class="myclose"></div>\n    <div class="close success-close-btn"></div>\n    <div class="dialog-left-greenbar"></div>\n    <div class="dialog-top">\n        <p class="doc-title" rel="new-dialog" alt="dialog_tit">{{=it.tmpdata.title}}</p>\n        </div>\n    <div class="dialog-inner tac">\n        <img class="ui-bz-avatar ui-bz-avatar-60-60" src="{{=it.tmpdata.step3.download_image}}" alt="" >\n        <p class="success">{{=it.tmpdata.step3.download_wenan}}\n            <a href="{{=it.tmpdata.step3.third_url}}" target="_blank" class="outlink log-xsend" data-logxsend="[1, 100897]">{{=it.tmpdata.step3.third_name}}</a>\n        </p>\n        </div>\n    {{? it.tmpdata.advertiser_step2}}\n    <div class="public-tip">\n        {{=it.tmpdata.advertiser_step2.before}}\n        <span class="name">{{=it.tmpdata.advertiser_step2.name}}</span>\n        {{=it.tmpdata.advertiser_step2.after}}\n        <span class="private">\u9690\u79c1\u4fdd\u969c<div class="hover-tip-hujiang"><span class="arrow-icon"></span>{{=it.tmpdata.privacy_policy}}</div></span>\n    </div>\n    {{??}}\n    <div class="public-tip">{{=it.tmpdata.advertiser}} <span class="private">\u9690\u79c1\u4fdd\u969c<div class="hover-tip-hujiang"><span class="arrow-icon"></span>{{=it.tmpdata.privacy_policy}}</div></span>\n        </div>\n    </div>\n    {{?}}\n{{?? it.list == 2}}\n    <div class="video-con">\n        <span class="myclose"></span>\n        <span class="video-close"></span>\n        <div class="header clearfix">\n            <span class="green-icon"></span><p class="video-title">{{= it.options.title}}</p><span class="video-size">&nbsp;(\u8bd5\u770b3\u5206\u949f)</span>\n        </div>\n        <div class="video" id="video">\n            \n        </div>\n        <div class="temp" style="display: none;">\n            <div class="video-wrap">\n\n            </div>\n            <p class="wenan">\u9884\u89c8\u7ed3\u675f</p>\n            <a href="{{= it.options.button_url}}" class="wrap-button" target="_blank">\u67e5\u770b\u5b8c\u6574\u89c6\u9891</a>\n        </div>\n        <div class="foot">\n            <img src="{{= it.options.video_img}}" alt="" class="video-jigou-icon">\n            <div class="content">\n                <div class="content-title">\n                    <span class="jigou-name">{{= it.options.release_user}}</span>&nbsp;\u63d0\u4f9b\u672c\u89c6\u9891\n                </div>\n                <p class="jigou-miaosu">{{= it.options.abstract}}</p>\n                <div class="video-tail-info">\n                    <span class="people-num">{{= it.options.view_count}}</span>\u4eba\u5df2\u89c2\u770b&nbsp;&nbsp;{{= it.options.section}}\n                </div>\n            </div>\n            <a href="{{= it.options.button_url}}"  target="_blank" title="" class="video-button">\u67e5\u770b\u5b8c\u6574\u89c6\u9891</a>\n        </div>\n    </div>\n{{?? it.list == 3}}\n<div class="ads-order-dialog">\n    <div class="header">\n        <div class="myclose"></div>\n        <div class="close list-close-btn"></div>\n        <h4>{{= it.data.title}}</h4>\n    </div>\n    <div class="container">\n        <img src="{{= it.data.banner}}" alt="">\n    </div>\n    <div class="footer">\n        <img src="{{= it.data.advertiser.advertiserLogo}}">\n        <p class="public-tip"><span>{{= it.data.advertiser.advertiserName}}</span>\u4e13\u4e1a\u63d0\u4f9b</p>\n        <p class="public-tip">{{= it.data.advertiser.advertiserTip}}</p>\n        <a href="###" class="ui-bz-btn-senior native-ads-download-btn">\n            <b class="ui-bz-btc ui-bz-btn-p-16">\u514d\u8d39\u9884\u7ea6</b>\n        </a>\n    </div>\n</div>\n\n{{?}}'),this.cacheData.downloadTpl=this.downloadTpl,this.init()});l.prototype={init:function(){var a=(s("head"),this);a.bindEvent(),window.oRawIncome=new c(a.cacheData)},showDialog:function(a,t){this.tempOptions=t;var i=this;if(3==a){o.xsend(1,101105,{id:t.adId,index:2});var n=this.getData(a,t);if(n){var c={};c.banner=i.cacheData.banner,c.title=i.cacheData.title,c.advertiser=i.cacheData.advertiser,c.privacy_policy=i.cacheData.privacy_policy,c.count=i.cacheData.count;var d=e.customDialog;s(".ys-ads-mask").show(),new d({width:464,height:332,content:i.downloadTpl({list:a,data:c}),closeSelector:".myclose",mask:{bgColor:"transparent"}})}}if(1==a){var n=this.getPlatInfo(t);if(n){var c={};c.doc_list=i.cacheList,c.title=i.cacheData.title,c.advertiser=i.cacheData.advertiser,c.privacy_policy=i.cacheData.privacy_policy,c.count=i.cacheData.count;var d=e.customDialog;s(".ys-ads-mask").show(),new d({width:464,height:1==a?500:332,content:i.downloadTpl({list:a,data:c}),closeSelector:".myclose",mask:{bgColor:"transparent"}})}}},bindEvent:function(){var a=this;s("body").on("click",".success-close-btn, .list-close-btn",function(){s(".myclose").trigger("click"),s(".ys-ads-mask").hide()}),s("body").on("click",".native-ads-download-btn",function(){s(".myclose").trigger("click"),window.oRawIncome.showDialog(),o.xsend(1,100895,{id:a.option.ad_id})}),s("body").on("click",".wrap-button",function(){o.xsend(1,100914,{ac_id:a.tempOptions.ad_id,res_time:Date.parse(new Date)-a.tempTime})}),s("body").on("keyup",function(a){27==a.keyCode&&(s(".myclose").trigger("click"),s(".ys-ads-mask").hide(),s(window).trigger("videoClose"))}),s("body").on("click",".success a",function(){o.xsend(1,100897,{id:a.option.ad_id})})},getData:function(a,t){var i=this;return i.cacheList||i.cacheData.advertiser_step2?!0:(d.getJSON(t.xpageId,{success:function(n){var c=e.customDialog;1==a?i.cacheList=n.model.step1.doc_list:3==a&&(i.cacheData.banner=n.model.step1.banner,i.cacheData.advertiser_step2=n.model.step2.advertiser),i.cacheData.index=a,i.cacheData.title=n.model.step1.title,i.cacheData.step2=n.model.step2,i.cacheData.step3=n.model.step3,i.cacheData.advertiser=n.model.step1.advertiser,i.cacheData.privacy_policy=n.model.step1.privacy_policy,i.cacheData.adname=n.wkrightcpl.dialog_page.step3.third_name,i.cacheData.download_tips=n.model.step1.download_tips,i.cacheData.adId=t.adId,i.cacheData.count=t.count,n.model.step1.count=t.count,s(".ys-ads-mask").show(),new c({width:464,height:1==a?564:332,content:i.downloadTpl({list:a,data:n.model.step1}),closeSelector:".myclose",mask:{bgColor:"transparent"}})}}),!1)},getPlatInfo:function(a){var t=this;if(t.cacheList||t.cacheData.advertiser_step2)return!0;var i=a.clickurl||"#";return t.visitUrl(i),s.ajax({type:"post",url:"https://eduad.baidu.com/cpl/wenku_post_json",data:JSON.stringify({slotid:"28",unitid:a.unitid,sid:a.sid,wkcplpage:"dialog",p:a.p,stm:a.stm}),contentType:"text/plain",dataType:"json",success:function(i){var n=e.customDialog;t.cacheData.index=1,t.cacheData.keyword=a.keyword,t.cacheData.city=a.city,t.cacheData.actionurl=i.wkrightcpl.actionurl,t.cacheData.unitid=a.unitid,t.cacheData.adname=i.wkrightcpl.dialog_page.step3.third_name,t.cacheList=i.wkrightcpl.dialog_page.step1.doc_list,t.cacheData.title=i.wkrightcpl.dialog_page.step1.title,t.cacheData.step2=i.wkrightcpl.dialog_page.step2,t.cacheData.step3=i.wkrightcpl.dialog_page.step3,t.cacheData.advertiser=i.wkrightcpl.dialog_page.step1.advertiser,t.cacheData.privacy_policy=i.wkrightcpl.dialog_page.step1.privacy_policy,t.cacheData.download_tips=i.wkrightcpl.dialog_page.step1.download_tips,t.cacheData.count=a.count,i.wkrightcpl.dialog_page.step1.count=a.count,s(".ys-ads-mask").show(),new n({width:464,height:500,content:t.downloadTpl({list:1,data:i.wkrightcpl.dialog_page.step1}),closeSelector:".myclose",mask:{bgColor:"transparent"}})},error:function(a){console.log(a)}}),!1},visitUrl:function(a){if(a){var t=new Image;return t.src=a,t}}},i.exports=l});