define("wkview:widget/html_view_newToolsbar/html_reader/html_reader_newtools.js",function(e,t,o){var n=e("wkcommon:widget/lib/tangram/base/base.js"),i=e("wkcommon:widget/ui/js_core/widget/widget.js").widget,r=e("wkcommon:widget/ui/js_core/log/log.js"),a=e("wkview:widget/html_view_newToolsbar/html_reader/js/newtoolsBar/toolsBar.js").toolsBar,s=e("wkview:widget/html_view/html_reader/js/zoom.js").zoom,l=e("wkview:widget/html_view/html_reader/js/fullScreen.js").fullScreen,d=e("wkview:widget/html_view/html_reader/js/page.js").page,g=e("wkview:widget/html_view/html_reader/js/praise.js").praise,c=e("wkview:widget/html_view/html_reader/js/favo.js").favo,p=e("wkview:widget/html_view/html_reader/js/appraise.js").appraise,w=e("wkview:widget/html_view_newToolsbar/html_reader/js/newtoolsBar/download.js").download,f=e("wkview:widget/doc_download_main/doc_download_main.js"),v=e("wkview:widget/html_view_newToolsbar/html_reader/js/newtoolsBar/pay.js").pay,u=(e("wkview:widget/html_view/html_reader/js/addToAlbum.js").addToAlbum,e("wkview:widget/html_view/html_reader/js/share.js").share),m=e("wkview:widget/html_view/html_reader/js/backToTop.js").backToTop,_=e("wkview:widget/html_view/html_reader/js/freeSendMobile.js").freeSendMobile,j=e("wkview:widget/html_view/html_reader/js/joinVip.js").joinVip,P=e("wkview:widget/html_view/html_reader/js/autofixed.js").autofixed,S=e("wkview:widget/html_view/html_reader/js/topMessage.js").topMessage,k=e("wkview:widget/html_view/html_reader/js/config.js").config,T=e("wkview:widget/html_view/html_reader/js/setting.js").setting,C=e("wkcommon:widget/ui/js_core/util/util.js"),b=e("wkcommon:widget/lib/fis/data/data.js"),R=0,O=(e("wkcommon:widget/ui/js_core/login/login.js").login,b.get("WkInfo")),L=e("wkcommon:widget/ui/js_core/xform/xform.js"),N=O.DocInfo,B=e("wkcommon:widget/ui/js_core/_dialog/_dialog.js").alert,E=e("wkview:widget/html_view/html_reader/js/free_hao123.js").freeHao123,A=e("wkcommon:widget/sessionX/sessionX.js"),z=A.sessionXMgr,V=i({Options:{el:"",doc_id:"",doc_id_update:"",docType:"doc",isLogin:0,zoomLevel:1,startPage:51,pageCount:100},_init:function(){var e=this;if(e.el){e.el=n.dom.g(e.el),e.pageList=n.dom.query(".bd",e.el),e.width=e.el.offsetWidth,e.height=e.el.offsetHeight,e.setting=T,k=C.mix(k,e.options,!0),e.config=k;var t=Math.min(e.startPage+k.pageStep-1,e.pageCount);if(O.DocInfo.isApi&&(t=e.pageRange+1),e.pageRange=[e.startPage,t],k.pageRange=e.pageRange,k.shareElId=e.shareElId,T.currentZoomLevel=e.zoomLevel,e.prevStartBtn=n.dom.g("prev-pageList-"+e.uid),e.nextStartBtn=n.dom.g("next-pageList-"+e.uid),e._scrollOfPage=null,e.secondClassify=N.cid3||"",k.bookmark=0===e.bookmark[0]?!1:e.bookmark,e.toPage>1&&e.gotoPage(e.toPage),T.currentPage=e.toPage,e.getPageFromPosition(),setTimeout(function(){n.lang.eventCenter.dispatchEvent("ReaderCreate.after",{reader:reader?reader:e.reader})},100),e.toolsBar=new a({readerContainer:e,reader:e.reader,items:e.getToolsBarItems()}),e.fullScreen=new l({readerContainer:e,toolsBar:e.toolsBar,reader:e.reader,status:"disabled"}),n.dom.addClass(e.fullScreen.el,"top-right-fullScreen"),e.el.appendChild(e.fullScreen.el),e.resize(),e.fire("onload"),e.bindEvent(),e.initFromName(),e.toPage>1){var o=new S({toolsBar:e.toolsBar,readerContainer:e});o.render()}e.updateScrollOverflow(),r.xsend(1,100037,{secondClassify:e.secondClassify,pn:e.toPage,out:0}),N&&"cqvip"===N.org_engName?r.xsend(1,100780,{index:1}):N&&1===~~N.hasImage&&-1===N.title.indexOf("\u56fe")&&r.xsend(1,100780,{index:2}),$(window).on("viewyuedurenwurukou",function(){L.getJSON("yuedudaoliu_flow",{success:function(e){0===e.yuedurenwu?$(".yuedurenwu_link").hide():1===e.yuedurenwu&&$(".yuedurenwu_link").show()}})})}},initFromName:function(){var e=this;1!==T.currentZoomLevel&&n.lang.eventCenter.dispatchEvent("Reader.initZoom"),e.nameToSetting()&&T.isFullScreen&&n.lang.eventCenter.dispatchEvent("Reader.initFullScreen")},updateScrollOverflow:function(){var e=n.page.getScrollTop(),t=document.body.scrollHeight,o=n.page.getViewHeight();T.scrollBof=0===e?!0:!1,T.scrollEof=e+o===t?!0:!1},bindEvent:function(){var e=this;n.lang.eventCenter.addEventListener("Reader.zoomChange",n.fn.bind(e.resize,e)),n.lang.eventCenter.addEventListener("Reader.zoomChange",n.fn.bind(e.pageResize,e)),n.lang.eventCenter.addEventListener("InFullScreen.before",n.fn.bind(e.setScrollOfPage,e)),n.lang.eventCenter.addEventListener("OutFullScreen.before",n.fn.bind(e.setScrollOfPage,e)),n.lang.eventCenter.addEventListener("InFullScreen.after",function(){return O.DocInfo.isApi?!1:(e.pageResize(),void e.gotoPage(e._scrollOfPage))}),n.lang.eventCenter.addEventListener("OutFullScreen.after",function(){e.pageResize(),e.gotoPage(e._scrollOfPage)});var t=0;n.lang.eventCenter.addEventListener("readMoreBtn.after",function(){0===t&&(e.gotoPage(4),t=1)}),n.lang.eventCenter.addEventListener("Reader.fullScreenResize",function(){e.pageResize(),e.reader.setZoom(e.el.offsetWidth)}),e.reader.addEventListener("firstRender",function(e,t){n.lang.eventCenter.dispatchEvent("Reader.firstRender",t)}),e.prevStartBtn&&n.event.on(e.prevStartBtn,"click",function(t){n.event.stop(t);var o=n.event.getTarget(t),i=+n.dom.getAttr(o,"data-prev-page");e.docRefresh(i)}),e.nextStartBtn&&n.event.on(e.nextStartBtn,"click",function(t){n.event.stop(t);var o=n.event.getTarget(t),i=+n.dom.getAttr(o,"data-next-page");e.docRefresh(i)});var o=C.debounce(function(){var t=n.page.getScrollTop();t!==R&&(R=t,e.getPageFromPosition(t))},100);n.event.on(window,"scroll",o),n.on(window,"unload",function(){try{e.reader.dispose()}catch(t){}}),$(".reader-end-download").find(".reader-download").on("click",function(e){e.preventDefault();var t=z.getSessionItem("fromSearchOptFirst"),o=$.extend(!0,{page:"view",pos:"reader"},t);r.xsend(101592,o);var n=N.isInRoomButDeletedByCreater?"\u6743\u9650\u95ee\u9898":"\u7248\u6743\u65b9\u8981\u6c42";if($(this).hasClass("downloadDisabled"))B({title:"\u63d0\u793a",content:'<p style="font-size:14px;margin:20px;text-align:center;">\u8be5\u6587\u6863\u56e0'+n+"\uff0c\u4ec5\u652f\u6301\u5728\u7ebf\u9605\u8bfb\u54e6\u3002</p>",width:350,height:180});else{var i=b.get("WkInfo");f.goSpecialDownload(i)}})},insertTopMsg:function(){var e=this;e.inner.appendChild(e.topMsg),n.event.on(e.topMsg,"click",function(){e.scrollToPosition(0,0)}),n.lang.eventCenter.addEventListener("Reader.scroll",function(t,o){o.isOverflow.Bof||(e.topMsg.style.top=n.page.getScrollTop()+"px")})},getPageFromPosition:function(e){var t,o,i,a=this,s=function(e,t){return t>e?-1:1},l=n.dom.getPosition(a.el).top;e=e&&e-l||n.page.getScrollTop()-l,a.createPagesTop(),o=a.pagesTop.concat([]),o.push(e),o.sort(s),t=n.array.indexOf(o,e),0>=t&&(t=1),i=Math.min(t+k.startPage-1,a.pageCount),T.currentPage!=i&&(r.xsend(1,100037,{secondClassify:a.secondClassify,pn:i,out:0}),T.currentPage=i),a.updateScrollOverflow(),n.lang.eventCenter.dispatchEvent("Reader.scroll",{currentPage:T.currentPage,top:e,isOverflow:a.isOverflow()})},getToolsBarItems:function(){var e=this,t=[l,s,g,c,p,function(){return e.isPrivate?null:u}(),d,E,function(){return e.isPayMent&&!e.ispaided?v:w}(),m,function(){if(b.get("searchQueryRefer"))return null;if(e.isPayMent){var t=O.DocInfo.goodsPayStatus;return showStatusList=["OtherActivity_NoDiscount_Vip","OtherActivity_Discount_Vip","OtherActivity_Free_Vip","OtherActivity_Only_Vip","NoneActivity_Discount_NoVip","NoneActivity_Free_NoVip","NoneActivity_Only_NoVip","OtherActivity_NoDiscount_NoVip","OtherActivity_Discount_NoVip","OtherActivity_Free_NoVip","OtherActivity_Only_NoVip","NoneActivity_NoDiscount_Vip","NoneActivity_Discount_Vip","PCActivity_NoDiscount_Vip","PCActivity_Discount_Vip"],n.array.indexOf(showStatusList,t)>-1?j:null}return _}()];return t},getPage:function(){return T.currentPage},isOverflow:function(){var e=this,t=n.dom.getPosition(e.el).top,o=n.page.getViewHeight(),i=e.el.offsetHeight,r=n.page.getScrollTop(),a=!1,s=!1;return t>=r&&(a=!0),r+o>=t+i&&(s=!0),{Bof:a,Eof:s}},createPagesTop:function(){var e=this,t=n.dom.getPosition,o=t(e.el).top,i=e.pageList,r=i.length,a=0,s=[];for(r;r>a;a++)s.push(t(i[a]).top-o);e.pagesTop=s,k.pagesTop=e.pagesTop},gotoPage:function(e){var t=this;if(O.DocInfo.isApi)return!1;if(!t.pageInRange(e))return void t.docRefresh(e);var o=n.page.getScrollLeft(),i=e%50-1,r=n.dom.getPosition(t.pageList[0>i?49:i]).top+50,a=t.getPageNo();n.lang.eventCenter.dispatchEvent("pageChange.before",{fromPageNo:a,toPageNo:e})&&(t.scrollToPosition(o,r),n.lang.eventCenter.dispatchEvent("pageChange.after",{fromPageNo:a,toPageNo:e}))},pageInRange:function(e){var t=this,o=t.pageRange;return e<o[0]||e>o[1]?!1:!0},docRefresh:function(e){var t=this,o=(t.setting,e),n=location.href.replace(/#/g,""),i=location.pathname,r=n.substr(n.indexOf(i)+i.length).indexOf("?"),a=n.indexOf("pn=");return t.setting.currentPage=e,t.settingToName(),1>o&&(o=1),o>k.pageCount&&(o=k.pageCount),-1===r?void(location.href=n+"?pn="+o):-1!==a?void(location.href=n.replace(/pn\=\d*/,"pn="+o)):void(location.href=n+"&pn="+o)},settingToName:function(e){var t=this,o=(window.name,{});e=e||{},o.readerSetting=n.object.extend(e,t.setting),window.name=n.json.stringify(o)},nameToSetting:function(){var e=window.name,t={};if(e&&-1!==e.search("readerSetting"))return t=n.json.parse(window.name),n.object.extend(T,t.readerSetting),window.name="",!0},scrollToPosition:function(e,t){window.scrollTo(e,t)},getPageNo:function(){return T.currentPage},prevScreen:function(){var e=n.page.getScrollLeft(),t=n.page.getScrollTop(),o=n.page.getViewHeight();window.scrollTo(e,t-o+140)},nextScreen:function(){var e=n.page.getScrollLeft(),t=n.page.getScrollTop(),o=n.page.getViewHeight();window.scrollTo(e,t+o-140)},resize:function(){var e=this;e.toolsBar.resize({readerWidth:e.el.offsetWidth,bodyElWidth:n.dom.query(k.bodySelector)[0].offsetWidth})},setScrollOfPage:function(){this._scrollOfPage=T.currentPage},pageResize:function(e,t){var o=this;o.pageResizeTimer&&clearTimeout(o.pageResizeTimer),o.pageResizeTimer=setTimeout(function(){o._pageResize(e,t)},15)},_pageResize:function(){try{var e=this,t=e.pageList,o=0,i=t.length;for(x=n.page.getScrollLeft(),y=n.page.getScrollTop(),h=n.page.getHeight(),pos=y/h,i;i>o;o++){var r=t[o],a=100*+parseFloat(n.dom.getAttr(r,"data-scale")).toFixed(2);"pay-page"!==r.id&&"bottom-download"!==r.id&&"longdoc-login-page"!==r.id&&(r.style.height=r.offsetWidth/a*100+"px")}h=n.page.getHeight(),y=h*pos,window.scrollTo(x,y),n.lang.eventCenter.dispatchEvent("Reader.pageResize")}catch(s){}},insert:function(e,t){var o=this,i=n.dom.q("reader-page",o.el,"div"),r=i.length;"undefined"==typeof t&&o.append(e),t-=1,t+1>r&&(t=r),0>t&&(t=0);var a=i[t];n.dom.insertAfter(e,a)},prepend:function(e){var t=this;n.dom.insertBefore(e,t.pageList[0])},append:function(e){var t=this;this.insert(e,t.pageList.length-1)}},{type:"html_reader"});(!n.browser.ie||n.browser.ie>7)&&new P({el:n.g("fixed-box")}),o.exports.html_reader=V});