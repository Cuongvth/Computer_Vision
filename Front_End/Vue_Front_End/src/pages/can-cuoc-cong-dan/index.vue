<script setup>
import { getCanCuoc, getCanCuocCount } from "./useAPI";
import { useStore } from "vuex";
import ShowDetail from "../ShowDetailData/showCard.vue";

const store = useStore();

var data = ref([]);
var lengthPage = ref(100);
var page = ref(1);

async function getData() {
  try {
    store.commit("setOverlayVisible", true);
    data.value = await getCanCuoc(page.value, 10);
    lengthPage.value = (await getCanCuocCount()) / 10 + 1;
    store.commit("setOverlayVisible", false);
    store.commit("setSnackBarContent", "Xác nhận thành công");
  } catch (error) {
    store.commit("setOverlayVisible", false);
    store.commit("setSnackBarContent", "Không tải được dữ liệu");
  }
}

watch(page, (newVal, oldVal) => {
  getData();
});

onMounted(() => {
  getData();
});
</script>

<template>
  <div>
    <VCard title="Dữ liệu chờ nhận dạng 🙌">
      <VCardText>Dữ liệu chờ nhận dạng</VCardText>
    </VCard>
    <VTable style="margin-top: 20px">
      <thead>
        <tr>
          <th class="text-left">
            Số thứ tự
          </th>
          <th class="text-left">
            No.
          </th>
          <th class="text-left">
            Full name
          </th>
          <th class="text-left">
            Date of birth
          </th>
          <th class="text-left">
            Sex
          </th>
          <th class="text-left">
            Nationality
          </th>
          <th class="text-left">
            Date of expiry
          </th>
          <th class="text-left">
            Date supply
          </th>
          <th class="text-left" />
        </tr>
      </thead>
      <tbody>
        <tr
          v-for="(item, i) in data"
          :key="item.id"
        >
          <td>{{ i + 1 }}</td>
          <td>{{ item.soCCCD }}</td>
          <td>{{ item.hoTen }}</td>
          <td>{{ item.ngayThangNamSinh }}</td>
          <td>{{ item.gioiTinh }}</td>
          <td>{{ item.quocTich }}</td>
          <td>{{ item.coGiaTriDen }}</td>
          <td>{{ item.ngayDangKy }}</td>
          <td>
            <ShowDetail
              :id="item.id"
              :type="1"
            />
          </td>
        </tr>
      </tbody>
    </VTable>
    <VPagination
      v-model="page"
      :length="lengthPage"
      :total-visible="10"
    />
  </div>
</template>
