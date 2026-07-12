import { NodeContent } from '@kit.ArkUI';

export interface ArkHelper {
  exit: (code: number) => void;
  requestPermission: (permission: string | string[]) => Promise<number | number[]>;
  openURL?: (url: string) => Promise<void>;
  showFileDialog?: (options: object) => Promise<object>;
  getWindowAvoidArea?: (type: number) => object | undefined;
}

export function init(context?: object): object;
export function render(helper: ArkHelper, slot: NodeContent): void;
export function initXComponent(xcomponent: object): number;
export function setResourceManager(resourceManager: object): number;
