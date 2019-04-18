// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import App from './App'
import router from './router'
import { ToastPlugin, ConfirmPlugin, AlertPlugin, Icon, XHeader,Swiper,SwiperItem,GroupTitle,Divider,XButton} from 'vux'
Vue.config.productionTip = false
Vue.use(ToastPlugin)
Vue.use(ConfirmPlugin)
Vue.use(AlertPlugin)
Vue.component('icon', Icon)
Vue.component('x-header', XHeader)
Vue.component('swiper', Swiper)
Vue.component('SwiperItem', SwiperItem)
Vue.component('Givider', GroupTitle)
Vue.component('Divider', Divider)
Vue.component('x-button', XButton)
/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  components: { App },
  template: '<App/>'
})
