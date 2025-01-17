import { setupLayouts } from "virtual:generated-layouts";
import { createRouter, createWebHashHistory } from "vue-router";
import routes from "~pages";

export default createRouter({
  history: createWebHashHistory(import.meta.env.BASE_URL),
  routes: [...setupLayouts(routes)],
});