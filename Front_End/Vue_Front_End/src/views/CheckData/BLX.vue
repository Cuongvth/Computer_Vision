<template>
  <VCard title="User Profile">
    <VCardText>
      <VForm @submit.prevent="onSubmit">
        <VRow>
          <VCol cols="9">
            <AppTextField
              readonly
              :value="props.cardData.so"
              density="compact"
              label="No."
              hint="No."
              variant="outlined"
            />
          </VCol>
          <VCol cols="3">
            <AppTextField
              v-model="checkTruong[0]"
              :rules="[
                requiredValidator,
                valid,
                betweenValidator(checkTruong[0], 0, 100),
              ]"
              density="compact"
              label="No."
              hint="No."
              variant="outlined"
            />
          </VCol>
          <VCol cols="9">
            <AppTextField
              readonly
              :value="props.cardData.hoTen"
              density="compact"
              label="Full name"
              hint="Full name"
              variant="outlined"
            />
          </VCol>
          <VCol cols="3">
            <AppTextField
              v-model="checkTruong[1]"
              :rules="[
                requiredValidator,
                valid,
                betweenValidator(checkTruong[1], 0, 100),
              ]"
              density="compact"
              label="Full name"
              hint="Full name"
              variant="outlined"
            />
          </VCol>
          <VCol cols="9">
            <AppTextField
              readonly
              :value="props.cardData.ngaySinh"
              density="compact"
              label="Date of birth"
              hint="Date of birth"
              variant="outlined"
            />
          </VCol>
          <VCol cols="3">
            <AppTextField
              v-model="checkTruong[2]"
              :rules="[
                requiredValidator,
                valid,
                betweenValidator(checkTruong[2], 0, 100),
              ]"
              density="compact"
              label="Date of birth"
              hint="Date of birth"
              variant="outlined"
            />
          </VCol>
          <VCol cols="9">
            <AppTextField
              readonly
              :value="props.cardData.noiCuTru"
              density="compact"
              label="Address"
              hint="Address"
              variant="outlined"
            />
          </VCol>
          <VCol cols="3">
            <AppTextField
              v-model="checkTruong[3]"
              :rules="[
                requiredValidator,
                valid,
                betweenValidator(checkTruong[3], 0, 100),
              ]"
              density="compact"
              label="Address"
              hint="Address"
              variant="outlined"
            />
          </VCol>
          <VCol cols="9">
            <AppTextField
              readonly
              :value="props.cardData.quocTich"
              density="compact"
              label="Nationality"
              hint="Nationality"
              variant="outlined"
            />
          </VCol>
          <VCol cols="3">
            <AppTextField
              v-model="checkTruong[4]"
              :rules="[
                requiredValidator,
                valid,
                betweenValidator(checkTruong[4], 0, 100),
              ]"
              density="compact"
              label="Nationality"
              hint="Nationality"
              variant="outlined"
            />
          </VCol>
          <VCol cols="9">
            <AppTextField
              readonly
              :value="props.cardData.hang"
              density="compact"
              label="Class"
              hint="Class"
              variant="outlined"
            />
          </VCol>
          <VCol cols="3">
            <AppTextField
              v-model="checkTruong[5]"
              :rules="[
                requiredValidator,
                valid,
                betweenValidator(checkTruong[5], 0, 100),
              ]"
              density="compact"
              label="Class"
              hint="Class"
              variant="outlined"
            />
          </VCol>
          <VCol cols="9">
            <AppTextField
              readonly
              :value="props.cardData.moTaXeDuocSuDung"
              density="compact"
              label="Vehicles are allowed to use"
              hint="Vehicles are allowed to use"
              variant="outlined"
            />
          </VCol>
          <VCol cols="3">
            <AppTextField
              v-model="checkTruong[6]"
              :rules="[
                requiredValidator,
                valid,
                betweenValidator(checkTruong[6], 0, 100),
              ]"
              density="compact"
              label="Vehicles are allowed to use"
              hint="Vehicles are allowed to use"
              variant="outlined"
            />
          </VCol>
          <VCol cols="9">
            <AppTextField
              readonly
              :value="props.cardData.ngayTrungTuyen"
              density="compact"
              label="Admission date"
              hint="Admission date"
              variant="outlined"
            />
          </VCol>
          <VCol cols="3">
            <AppTextField
              v-model="checkTruong[7]"
              :rules="[
                requiredValidator,
                valid,
                betweenValidator(checkTruong[7], 0, 100),
              ]"
              density="compact"
              label="Admission date"
              hint="Admission date"
              variant="outlined"
            />
          </VCol>
          <VCol cols="9">
            <AppTextField
              readonly
              :value="props.cardData.ngayDangKy"
              density="compact"
              label="Registration date"
              hint="Registration date"
              variant="outlined"
            />
          </VCol>
          <VCol cols="3">
            <AppTextField
              v-model="checkTruong[8]"
              :rules="[
                requiredValidator,
                valid,
                betweenValidator(checkTruong[8], 0, 100),
              ]"
              density="compact"
              label="Registration date"
              hint="Registration date"
              variant="outlined"
            />
          </VCol>
          <VCol cols="12">
            <VBtn type="submit">
              Save
            </VBtn>
          </VCol>
        </VRow>
      </VForm>
    </VCardText>
  </VCard>
</template>

<script setup>
const props = defineProps({
  cardData: Object,
  setCheck: Function,
});

import { betweenValidator, requiredValidator } from "@validators";
import { useStore } from "vuex";

var checkTruong = ref([
  props.cardData.isSo,
  props.cardData.isHoTen,
  props.cardData.isNgaySinh,
  props.cardData.isNoiCuTru,
  props.cardData.isQuocTich,
  props.cardData.isHang,
  props.cardData.isMoTaXeDuocSuDung,
  props.cardData.isNgayTrungTuyen,
  props.cardData.isNgayDangKy,
]);
const store = useStore();

function valid(value) {
  const regex = /^([0-9]|[1-9][0-9]|100)$/;

  return regex.test(value);
}

function onSubmit() {
  var ok = true;

  checkTruong.value.forEach(element => {
    if (!valid(element)) {
      ok = false;
    }
  });

  if (!ok) {
    store.commit("setSnackBarContent", "Thất bại");

    return;
  }
  props.setCheck(3, Array.from(checkTruong.value));
  store.commit("setSnackBarContent", "Lưu thành công");
}
</script>
